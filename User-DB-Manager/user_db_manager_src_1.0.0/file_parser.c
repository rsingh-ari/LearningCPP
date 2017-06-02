/*---------------------- Copyright (c) Picarro, 2017 -------------------
 *                                 Source code
 * This software is the property of Picarro and should be considered and treated
 * as proprietary information.  Refer to the "Source Code License Agreement"
 *----------------------------------------------------------------------------
*/

/** @brief File parser program interfaces
 *  @file file_parser.c
 *  @author Aricent
 *  @date Apr 2017
 *  @version 1.0
 *  
 *  This file parse the CH4 calibration data files, perform the calculations
 *  and store them to database file. 
 *  
 */ 

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "db_management.h"
#include "file_parser.h"
#include "plot_graph.h"
#include "debug.h"
#include "pdf_generator.h"


#define DIR_NAME 10
#define FULL_PATH 100
#define FILE_PFIX_LEN 10
#define FULL_FILE_NAME 50
#define MAX_SPACE 15
#define MAX_LEN_NUM 25
#define MAX_GAS_SOURCE 4


unsigned int total_file_count;
unsigned int total_config_calib;

struct dat_fileInfo{
        char file_name[50];
        char filepath[100];
		char fileprefix[100];
        char available_ppm[10];
        char file_header[10];
		int total_calib_values;
		float calib_values_sum;
        float concentration_mean;     // y-axis
        float nominal_concentration; // x-axis
		float standard_deviation;
        float calibration_values[1000];
};

struct calib_key_value
{
        char key[10];
        float value;
};

struct config_file_struct
{
        char filePath[256];
        char fileHeader[15];
        char filePrefix[50];
        int calFileCount;
        struct calib_key_value calibArray[50];
};

struct config_file_struct conf_settings = {0};
struct dat_fileInfo *f_info = NULL;
void perform_sorting(char *gas_src[],float *Nmh4ppm,float *SD,float *observeconcentration_mean);


/** @brief Parse config file 
 *
 *  This function parse the calibration config file and fill the 
 *  values in config settings.
 * 
 *  @return Void .
 */
void parse_config_file()
{
  char config_key[50][15];
  char line[256];
  char calib_count[10];
  FILE *fPtr = fopen("/usr/local/bin/user_db_config/user_db_mgr_config.conf" , "r");  
  int key_index=0,key_val_index=0;  
  int i=0,len=0,j=0,eflag=0;
  
  memset(config_key,'\0',50*15);
  memset(&conf_settings, 0, sizeof(conf_settings));
  memset(conf_settings.filePath,'\0', sizeof(conf_settings.filePath));
  memset(conf_settings.fileHeader, '\0', sizeof(conf_settings.fileHeader));
  memset(conf_settings.filePrefix, '\0', sizeof(conf_settings.filePrefix));
  memset(conf_settings.filePrefix, '\0', sizeof(conf_settings.filePrefix));
  
  for (i=0;i<50;i++)
  {
	memset(conf_settings.calibArray[i].key, '\0', sizeof(conf_settings.calibArray[i].key));
	conf_settings.calibArray[i].value=0.0;
  }
  
  while (fgets(line, sizeof(line), fPtr))
  {
     
	i=0;
	j=0;
	eflag=0;
	len=strlen(line);
	memset(calib_count,'\0',10);
	while(i<len)
	{
		/*skip the line started with '#' and blank line*/
		if(*(line+0) == '#' || *(line+0)=='\r' || *(line+0)==' ' || *(line+0) == '\t')
				{
				int hello;
				break;
				}
		if(*(line+i) == '\n')
		{                
			break;
		}
		else
		{
			if(line[i] != ' ' || line[i] != '\t')
			{
				if(line[i] == '=')
				{
					j=0;
					eflag = 1;
				}
				else
				{
					if(eflag)
					{
						if(key_index == 0)
						{
							conf_settings.filePath[j]=line[i];
							j++;
						}
						else if(key_index == 1)
						{
							conf_settings.fileHeader[j]=line[i];
							j++;
						}
						else if(key_index == 2)
						{
							conf_settings.filePrefix[j]=line[i];
							j++;
						}
						else if(key_index == 3)
						{
							if(line[i]!='\r'|| line[i+1]!='\n' )
								calib_count[j]=line[i];
							if(line[i] == '\r' || line[i+1]=='\n')
							{
								conf_settings.calFileCount=atoi(calib_count);
							}
							j++;
						}
						else
						{
							if(line[i]!='\r' || line[i+1]!='\n')
								calib_count[j]=line[i];
							if(line[i] == '\r' || line[i+1] == '\n')
							{
								conf_settings.calibArray[key_val_index++].value = atof(calib_count);
							}
							j++;
						}
					}
					else
					{
						if(key_index == 0)
						{
							config_key[key_index][j]=line[i];
							j++;
						}
						else if(key_index == 1)
						{
							config_key[key_index][j]=line[i];
							j++;
						}
						else if(key_index == 2)
						{
							config_key[key_index][j]=line[i];
							j++;
						}
						else if(key_index == 3)
						{
							config_key[key_index][j]=line[i];
							j++;
						}
						else
						{
							config_key[key_index][j]=line[i];
							conf_settings.calibArray[key_val_index].key[j] = line[i];
							j++;
						}
					}
				}                
			}
		}
		i++;

	}
	if(*(line+0) != '#' && *(line+0) != '\r' && *(line+0)!=' ' && *(line+0)!='\t' && *(line+0)!='\n')
	{
		key_index++;
	}

	memset(line,'\0',256);
   }
   fclose(fPtr);      
}

/** @brief Get available concentration key from filename in the f_info structure. 
 *
 *  This function get the concentration key from the file name existing in the given directory. 
 * 
 *  @param total_file_count total calibration files in a directory given in the command.
 *  @param f_info structure to represent a single file. 
 * 
 *  @return Void .
 */
void get_available_concentration(int total_file_count,struct dat_fileInfo *f_info)
{
        int k=0;
        char *z_air = "zero_air";
        int z_len = strlen(z_air);
        int hypen_flag = 0 ,underscore_flag=0;
        while(k<total_file_count)
        {
			char ch;
			int i=0;
			int j=0;
			hypen_flag = 0;
			underscore_flag=0;
			do
			{
				ch = *((f_info+k)->file_name+i);
				if(ch=='-')
				{
					hypen_flag++;
				}
				if(hypen_flag == 2)/*MAX Number of '-' in a file name*/
				{
					if(strncmp((f_info[k].file_name+i+1),z_air,z_len)==0)
					{
						strncpy((f_info+k)->available_ppm,(f_info[k].file_name+i+1),z_len);
						break;
					}
					if(ch == '_')
					{
						underscore_flag++;
					}
					else
					{
						if(underscore_flag == 1)
						{
							(f_info+k)->available_ppm[j]=ch;
							j++;
						}
					}
				}
					i++;
			}while(*((f_info+k)->file_name+i)!='.');
			
			if(strcmp(f_info[k].available_ppm,z_air) !=0 )
				(f_info+k)->available_ppm[j]='\0';
			
			k++;
        }
}

/** @brief Create dat files info structure instance for every file.
 *
 *  This function create dat files info structure instance for every file available in the given directroy. 
 * 
 *  @param total_files total calibration files in directory. 
 * 
 *  @return struct files info structure.
 */
struct dat_fileInfo* create_dat_fileInfo(int total_files)
{
	struct dat_fileInfo *f_info = (struct dat_fileInfo*)malloc((sizeof(struct dat_fileInfo)*total_files));		
	if(f_info !=NULL)
	{
		memset(f_info,0,(sizeof(struct dat_fileInfo)*total_files));			
		return f_info;
	}
	else
	{			
		return NULL;
	}
}


/** @brief Scan the current directory and read the all files name from given directory.
 *
 *  This function scan the current directory and read the all files name from given directory and
 *  update the file name filed in the file info structure which represent a single file.
 * 
 *  @param dir_name name of the given directory in the json command. 
 * 
 *  @return struct filename.
 */
struct dat_fileInfo* get_all_files(char *dir_name)
{
		char file_prefix[50]="NBDS2039-";
        char tmp_fname[50][50];
        unsigned int total_calib_files=0;
        DIR *dir;
        struct dirent *ent;
        struct dat_fileInfo *f_info = NULL;
		char full_path[FULL_PATH];
		memset(full_path,'\0',sizeof(full_path));
		strcpy(full_path,conf_settings.filePath);
		strcat(full_path,dir_name);
		if((dir = opendir(full_path)) != NULL) 
		{
			/* print all the files and directories within directory */
			while ((ent = readdir (dir)) != NULL) 
			{
				if((strncmp(ent->d_name,"NBDS2039-",strlen("NBDS2039-")))==0)
				{
					strcpy(tmp_fname[total_calib_files],ent->d_name);
					//printf("%s\t%s\n",tmp_fname[total_calib_files] ,ent->d_name);
					total_calib_files++;
				}
			}
			closedir (dir);
			if(total_calib_files > 0)
			{
				int k = 0;
				total_file_count = total_calib_files;
				f_info =  create_dat_fileInfo(total_calib_files);

				while(k < total_calib_files)
				{
					strcpy((f_info+k)->file_name,tmp_fname[k]);					
					k++;
				}
				return f_info;
			}
			else
			{
				return f_info;
			}
        } 
		else
		{
			/* could not open directory */
			printf ("ERROR :No such file or directory\n");
			return NULL;
        }		
}


/** @brief Scan the current directory and read the all files name from given directory.
 *
 *  This function scan the current directory and read the all files name from given directory and
 *  update the file name filed in the file info structure which represent a single file.
 * 
 *  @param dir_name name of the given directory in the json command. 
 * 
 *  @return struct filename.
 */
int find_concentration_offset(char *fheader,int idx)
{
	char *p,*token;
	unsigned int offset=0;
	char *delimit= " ";	
	token = strtok(fheader,delimit);
	offset++;
	if(strcmp(token,(f_info+idx)->file_header)==0)
	{
		return offset;
	}
	else
	{
		while(token)
		{
		  token = strtok(0,delimit);
		  offset++;
		  if(strcmp(token,(f_info+idx)->file_header)==0)
		  {
			break;
		  }
		}
	}	
	//printf("offset= %d\n\n",offset);
	return offset;

}

/** @brief Make file name from directory name
 *
 *  This function makes file name from directory name.
 * 
 *  @param file_name Output buffer. 
 *  @param dir_name Name of the directory. 
 *  @param index Used for file postfix buffer.
 *  @return Void.
 */
void get_filename(char* file_name,int index,char *dir_name)
{
	char file_prefix[FILE_PFIX_LEN]="NBDS2039-";
	char *file_posfix[4]={"-zero_air","-ch4_2ppm","-ch4_10ppm","-ch4_100ppm"};	
	/* file name*/
	strcpy(file_name,file_prefix);
	strcat(file_name,dir_name);
	strcat(file_name,file_posfix[index]);
	strcat(file_name,".dat");
#if DEBUG
	print("get_filename file_name11 = %s\n",file_name);
#endif
}

/** @brief Make directory name from directory path.
 *
 *  This function makes directory name from directory path.
 * 
 *  @param file_name Output buffer. 
 *  @param dir_name Output buffer. 
 *  @param pdf_dir_path Used as input buffer.
 *  @return Void.
 */
void get_dir_name(char *pdf_dir_path,char *file_name,char *dir_name)
{
	int i;
	int len1 = strlen(pdf_dir_path);
	char *temp = pdf_dir_path;
	int len2 = strlen("20170222/");
	temp = (temp +(len1 - len2 ));
	for(i=0;i<len2;i++)
	{
		file_name[i] = temp[i];
	}		
}

/** @brief Split string.
 *
 *  This function split the file name and path..
 * 
 *  @param path path of the file.
 *  @param fname File name buffer. 
 *  @return Void.
 */
void split_string(char *path,char* fname)
{
	int len1,len2;
	len1 = strlen(path);
	len2 = strlen(fname);
	memset((path+(len1-len2)),'\0',len1);	
	//print("path=%s\tlen=%d\n",path,len1);	
}

/** @brief Update dat_fileInfo structure from the conf_settings structure for every file.
 *
 *  This function updates dat_fileInfo structure from the conf_settings structure for every file.
 * 
 *  @param path path of the file.
 *  @param fname File name buffer. 
 *  @return Void.
 */
void set_file_configsetting(int total_files)
{
	int i=0;
	int j=0;
	int flag = 0;	
	total_config_calib = conf_settings.calFileCount;
	for(i=0;i<total_files;i++)
		{
		strcpy((f_info+i)->filepath,conf_settings.filePath);
		strcpy((f_info+i)->fileprefix,conf_settings.filePrefix);
		strcpy((f_info+i)->file_header,conf_settings.fileHeader);
		}
	while(j<total_files)
	{
		for(i=0;i<conf_settings.calFileCount;i++)
		{
		if(strcmp(conf_settings.calibArray[i].key,(f_info+j)->available_ppm) == 0)
			{
			(f_info+j)->nominal_concentration = conf_settings.calibArray[i].value;
			flag = 1;
			break;
			} 
		}
		if(!flag)
		{
		char *tmp = (f_info+j)->available_ppm;
		float val=0.0;
		while( *tmp != '\0' )
			{
				if(*tmp >= 48 && *tmp <= 57)
				{
					val = (val*10 + (*tmp-48)) ;
				}
			tmp++;
			}
		(f_info+j)->nominal_concentration = val;	
		}
		j++;
	}	

}

/** @brief Display the dat_fileInfo structure values.
 *
 *  This function display the dat_fileInfo structure values.
 *   
 *  @return Void.
 */
void display_data()
{
	int i;
	for(i=0;i<4;i++)
		{
			printf("file_name = %s \n", f_info[i].file_name);
			printf("filepath = %s \n", f_info[i].filepath);
			printf("fileprefix = %s \n", f_info[i].fileprefix);
			printf("available_ppm = %s \n", f_info[i].available_ppm);
			printf("file_header = %s \n", f_info[i].file_header);
			printf("nominal_concentration = %f \n", f_info[i].nominal_concentration);
		}
}

/** @brief Create validation report 
 *
 *  This function parse the config file and all calibration files and create the validation report.
 * 
 *  @param date_time Current date for validation.
 *  @param user_name name of the validator.
 *  @param folder_name Name of the latest validation directory.
 *  @return Char* String returns as pdf report.
 */
char* get_ch4_ppm_update_db(char *date_time,char *user_name,char *folder_name )
{
	float Nmh4ppm[50]={0.0};
	float SD[50] = {0.0};
	float observeconcentration_mean[50]={0.0};
	int len=0,loop_index=0;
	char *gas_src[50];
	char *pdf_fName=NULL;	
	char full_path[FULL_PATH];
	char pdf_dir_path[100];
	FILE *fp;
	
	//printf("Inserting calibration data in db..........");
	memset(Nmh4ppm,0.0,sizeof(Nmh4ppm));
	memset(SD,0.0,sizeof(SD));
	memset(observeconcentration_mean,0.0,sizeof(observeconcentration_mean));
	memset(pdf_dir_path,'\0',sizeof(pdf_dir_path));
	
	parse_config_file();
	//get_filename(file_name,zero_air,dir_name);	
	f_info = get_all_files(folder_name);
	get_available_concentration(total_file_count,f_info);
	set_file_configsetting(total_file_count);
#if DEBUG	
	//display_data();
#endif
	
	//Openning calibrations files one by one
	loop_index=0;
	while(loop_index<total_file_count)
	{
		int conc_offset=0;
		char *r_val;
		char buf[1000];
		int total_val=0;
		int row_val_index=0;
		
		memset(buf,'\0',sizeof(buf));
		memset(full_path,'\0',sizeof(full_path));
		
		strcpy(full_path,conf_settings.filePath);
		strcat(full_path,folder_name);
		strcat(full_path,"/");
		strcat(full_path,(f_info+loop_index)->file_name);
#if DEBUG		
		printf("filepath =%s\n",full_path);
#endif		
		fp = fopen(full_path,"r+");
		if(!fp)
		{
			printf("File %s not opened\n",(f_info+loop_index)->file_name);	
		}
		
		//Reading headers from all callibration files
		r_val=fgets(buf,sizeof(buf),fp);	
		conc_offset = find_concentration_offset(buf,loop_index);
		while(r_val !=NULL)
		{
			r_val=fgets(buf,sizeof(buf),fp);
			if(r_val!=NULL )
			{
				fill_values(date_time,user_name,buf,conc_offset,loop_index,row_val_index);
				row_val_index++;
			}
			if(r_val==NULL )
			{
				(f_info+loop_index)->total_calib_values = row_val_index;
			}
			
		}
		fclose(fp);
		loop_index++;
	}
	for(loop_index=0;loop_index<total_file_count;loop_index++)
	{
		(f_info+loop_index)->concentration_mean=(float)((f_info+loop_index)->calib_values_sum/(f_info+loop_index)->total_calib_values);
#if DEBUG
		//printf("total=%d\tsum=%f\tavg=%f\n",(f_info+loop_index)->total_calib_values,(f_info+loop_index)->calib_values_sum,(f_info+loop_index)->concentration_mean);
#endif
	}
	for(loop_index=0;loop_index<total_file_count;loop_index++)
		calculate_SD(loop_index);
	
	/* Taking Nominal concentration ,concentration mean and standard_deviation in local array. */
	for(loop_index=0;loop_index<total_file_count;loop_index++)
	{
		Nmh4ppm[loop_index] = (f_info+loop_index)->nominal_concentration;
		SD[loop_index] = (f_info+loop_index)->standard_deviation;
		observeconcentration_mean[loop_index] = (f_info+loop_index)->concentration_mean;
		gas_src[loop_index] = (f_info+loop_index)->available_ppm;
#if DEBUG		
		//printf("Nmh4ppm=%f\tSD=%f\tmean=%f\n",Nmh4ppm[loop_index],SD[loop_index],observeconcentration_mean[loop_index]);
#endif		
	}
	
	perform_sorting(gas_src,Nmh4ppm,SD,observeconcentration_mean);
	/******
	TBD: Insert values in database tables
	*******/	
	//insert_gas_table((void*)(unsigned long long)get_dta_base(),date_time,user_name,folder_name,gas_src[loop_index],Nmh4ppm[loop_index],avg[loop_index],SD[loop_index]);	
	
    plot_graph(Nmh4ppm,observeconcentration_mean,total_file_count);
	sleep(1);	
	pdf_fName = generate_pdf(gas_src,observeconcentration_mean,Nmh4ppm,SD,total_file_count);
	
	if(pdf_fName != NULL)
	{	
		char cmd[100];
		struct stat sb;
		memset(cmd,'\0',100);
		strcpy(pdf_dir_path,conf_settings.filePath);
		strcat(pdf_dir_path,"Report");
		if (stat(pdf_dir_path, &sb) == 0 && S_ISDIR(sb.st_mode))
		{
#if DEBUG			
			printf("pdf_dir_path=%s\tpdf_fanem=%s\n",pdf_dir_path,pdf_fName);
#endif			
			sprintf(cmd,"mv %s %s",pdf_fName,pdf_dir_path);
			system(cmd);	
		}
		else
		{
			sprintf(cmd,"mkdir %s",pdf_dir_path);
			system(cmd);
#if DEBUG			
			printf("pdf_dir_path=%s\tpdf_fanem=%s\n",pdf_dir_path,pdf_fName);
#endif			
			memset(cmd,'\0',100);
			sprintf(cmd,"mv %s %s",pdf_fName,pdf_dir_path);
			system(cmd);
		}
	}
	if(f_info != NULL)
	{
		free(f_info);
		f_info = NULL;
	}
	return pdf_fName;
}


/** @brief Based on the column offset of the header given in the config file 
 *  this function extract the ppm values from the row in the given input buffer.
 *
 *  This function extract the ppm values of that row from the calibration file according to header given in the configration file.
 * 
 *  @param date_time Date time value 
 *  @param user_name user name of the validator
 *  @param input_buf Line read from the calib file
 *  @param col_offset Offset of the header given in the config file.
 *  @param fnum Index of the file in which filling the values in the file info structure.
 *  @param row_val Index of the calibration value array.
 * 
 *  @return Void.
 */
void fill_values(char *date_time,char *user_name, char *input_buf,int col_offset,int fnum,int row_val)
{

		int i=0,row_offset=0;
		static char *delimit = " ";
		char *token = strtok(input_buf,delimit);
		row_offset++;
        if(row_offset == col_offset)
		{
		/*do nothing*/
		}
		else
		{
        	while(token)
        	{
                token = strtok(0,delimit);
				row_offset++;
                if(row_offset == col_offset)
				break;
            }
		}
		if(row_offset == col_offset)
		{
			char tmp[25] ;//= (char*)malloc(sizeof(char)*MAX_LEN_NUM);
			memset(tmp,'\0',sizeof(tmp));
			while(token[i] != '\0')
			{	
				 tmp[i]= token[i];
				 i++;
			}
			(f_info+fnum)->calibration_values[row_val] = (float)atof(tmp);
			(f_info+fnum)->calib_values_sum += (f_info+fnum)->calibration_values[row_val];
			//printf("calib_value = %lf\n",(f_info+fnum)->calibration_values[row_val]);
			//free(tmp);
		}
			
}

/** @brief perform_sorting. 
 *
 *  This function perform sorting on the basis of Nominal ch4 concentration value 
 * 
 *  @param gas_src array of char pointers. 
 *  @param Nmh4ppm float pointer.
 *  @param SD float pointer.
 *  @param observeconcentration_mean float pointer.
 *  @return void.
 */
void perform_sorting(char *gas_src[],float *Nmh4ppm,float *SD,float *observeconcentration_mean)
{
	int i=0,j=0;
	float tmp_1;
	char *tmp_2;
	
	for(i=0;i<total_file_count-1;i++)
	{
		j=i+1;
		tmp_1=0.0;
		while(j>0)
		{
			if(Nmh4ppm[j]<Nmh4ppm[j-1])
			{
				tmp_1 = Nmh4ppm[j];
				Nmh4ppm[j] = Nmh4ppm[j-1];
				Nmh4ppm[j-1] = tmp_1;
				
				tmp_2 = gas_src[j];
				gas_src[j] = gas_src[j-1];
				gas_src[j-1] = tmp_2;
				
				tmp_1 = SD[j];
				SD[j] = SD[j-1];
				SD[j-1] = tmp_1;
				
				tmp_1 = observeconcentration_mean[j];
				observeconcentration_mean[j] = observeconcentration_mean[j-1];
				observeconcentration_mean[j-1] = tmp_1;
				
			}
			j--;
		}
	}
	/*for(i=0;i<total_file_count;i++)
	{
		printf("Nmh4ppm[%d]=%f\n",i,Nmh4ppm[i]);
	}*/
}


/** @brief Square. 
 *
 *  This function squares the given value..
 * 
 *  @param b Float value. 
 *  @return float float value..
 */
float square(float b)
{
    float z;
    z = b*b;
    //printf("%f %f",z ,b);
    return(z);
}

/** @brief Calculate SD. 
 *
 *  This function calculates the standard daviasions.
 * 
 *  @param avg[] Mean of the calibration.
 *  @return Void.
 */
void calculate_SD(int f_index)
{
	int calib_index=0;
	float diff=0.0,sqr_sum=0.0;
	for(calib_index=0;calib_index <(f_info+f_index)->total_calib_values;calib_index++)
	{
		diff = (f_info+f_index)->calibration_values[calib_index]-(f_info+f_index)->concentration_mean;
		if(diff < 0)
		  diff = fabsf(diff);
		sqr_sum += square(diff);
	}
	(f_info+f_index)->standard_deviation = sqrt(sqr_sum/(f_info+f_index)->total_calib_values);
}
