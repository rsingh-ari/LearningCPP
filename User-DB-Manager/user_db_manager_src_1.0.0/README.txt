
/------------------------Compilation procedure--------------------------------/

 1.	Extract the user_db_manager_src_1.0.0.tar.gz file.
 2.	Open the file user_db_mgr_config.conf and add the path of ppm data files in this config file.
 3.	Follow the instruction to update the user_db_mgr_config.conf file.
 4.	This config (user_db_mgr_config.conf) file is provided for the flexibility of report generation.
 5.	Run the script user_db_install_lib.sh.
 6.	Now build the directory with following commands:
	 	1.	make clean
	 	2.	make
	 	3.	make install
 7.	Restart the system.
 8.	On system boot up user_db_manager program would be running in background.
 9.	Program running can be check with command: 
		ps -ax | grep user_db_manager
 10.Now run the client utility (./test_db_manager) to communicate with User and DB manager program.



/ -------------USE FOLLOWING INSTRUCTIONS TO UPDATE THE CONFIG (user_db_mgr_config.conf)FILE--------------------/

 1. Do not add blank space/tab at the beginning of any line.
 2. Always start a comment with # initials.
 3. Config file must be filled with valid values for first four keys : FILE_PATH, HEADER, FILE_PREFIX,FILE_COUNT.

 KEY DEFINITIONS: 
 
 1. FILE_PATH   : Path to the calibration data files before the date folder name. Date folder name would be passed as 
                  parameter in careaValidationReport command.

 2. HEADER      : This is the header name of GAS for which validation report would be generated. This can be changed to H2O or H2O2 etc.
 
 3. FILE_PREFIX : This is the prefix to add in the file name while creating the .dat file names Eg. NBDS2039-20170222-ch4_100ppm.dat.
 
 4. FILE_COUNT  : This count will contain the number of nominal concentration defined in this configration file. 
                  This will used to plot X-axis data points on the graph. The concentration key=value pair and FILE_COUNT should be matched.

 5. KEY=VALUE pair: This will define nominal concentration as a KEY=VALUE pair.
                    The KEY would be used to create/compare with the file name available in the calibration file folder.

 	Example: 
 	•	If ppm data files for CH4 gas are in directory /home/ch4-ppm-calibration//20170305/
 	•	Add the config file keys as: 
 	•	FILE_PATH=/home/ch4-ppm-calibration/
 	•	HEADER=CH4
 	•	FILE_PREFIX=NBDS2039-
 	•	FILE_COUNT=4
 	•	zero_air=0.0
 	•	2ppm=2.058
 	•	10ppm=10
 	•	100ppm=100.2
 	•	Pass the latest validation directory name (20170305) as json request from test_db_manager application in createValidationReport command.
 		E.g.: {\"command\": \"createValidationReport\", \"validationTime\": \"170424 02:58\",  \"validationDirName\": \"20170222\"}                    
 

/---------------------Testing with test_db_manager program---------------------------/
 Run client program (test_db_manager) on the terminal
 Run List user.
 Run ADD user command.
 Run List user.
 Run Login user.
 Run Get current user.
 Run Create validation report.
 Run Logout user.
 Run Get current user.
 Check the DB file for updates.

/---------------Path of generated database and pdf report file -----------------------------------/

 Path of generated database file and copied user_db_mgr_config.conf file:
   /use/local/bin/user_db_config/user_db_manager.db
   /use/local/bin/user_db_config/ user_db_mgr_config.conf
 Path of generated report:
   /home/ch4-ppm-calibration/Report/May10-3-validation.pdf

/----------------List of commands used by test_db_manager---------------/

 1. Add User Commands:
 {\"command\": \"addUser\", \"username\": \"user1\", \"role\": \"admin\", \"password\": \"password\"} {\"command\": \"addUser\", \"username\": \"user2\", \"role\": \"operator\", \"password\": \"password\"} {\"command\": \"addUser\", \"username\": \"user3\", \"role\": \"operator\", \"password\": \"password\"} {\"command\": \"addUser\", \"username\": \"user4\", \"role\": \"operator\", \"password\": \"password\"}
 
 2. DELETE Commands:
 {\"command\": \"deleteUser\", \"username\": \"user1\", \"role\": \"admin\"}
 {\"command\": \"deleteUser\", \"username\": \"user2\", \"role\": \"operator\"} 
 {\"command\": \"deleteUser\", \"username\": \"user3\", \"role\": \"operator\"} 
 {\"command\": \"deleteUser\", \"username\": \"user4\", \"role\": \"operator\"}
 
 3. Login Commands:
 {\"command\": \"loginUser\", \"username\": \"user1\", \"password\": \"password\"}
  {\"command\": \"loginUser\", \"username\": \"user2\", \"password\": \"password\"}
  {\"command\": \"loginUser\", \"username\": \"user3\", \"password\": \"password\"} 
 {\"command\": \"loginUser\", \"username\": \"user4\", \"password\": \"password\"}
 
 4. List User Command:
  {\"command\": \"listAllUsers\"}
 
 5. Log Out  User Command:
 command\": \"logoutUser\"}
 
 6. Current user Command:
 {\"command\": \"getCurrentUser\"}
 
 7. Report Generation Command:
 {\"command\": \"createValidationReport\", \"validationTime\": \"170424 02:58\", \"validationDirName\": \"20170222\"}


/-------------------------------------------------------------------------------------------------------------------/
