/*---------------------- Copyright (c) Picarro, 2017 -------------------
 *                                 Source code
 * This software is the property of Picarro and should be considered and treated
 * as proprietary information.  Refer to the "Source Code License Agreement"
 *----------------------------------------------------------------------------
*/

/** @brief generate pdf program
 *  @file pdf_generator.c
 *  @author Aricent
 *  @date Apr 2017
 *  @version 1.0
 *  
 *  This file takes the input from file parser and generate the PDF report of that values.
 *  
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include <time.h>
#include <hpdf.h>
#include <grid_sheet.h>

#include "pdf_generator.h"

jmp_buf env;


/** @brief Register error hanlder
 *
 *  This function handle errors of PDF library.
 *  
 *  @return Void.
 */
#ifdef HPDF_DLL
void __stdcall
#else
void
#endif
error_handler  (HPDF_STATUS   error_no,
                HPDF_STATUS   detail_no,
                void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}


/** @brief Print text.
 *
 *  This function print the positions of x and y cordinates on a page.
 * 
 *  @param page Variable to HPDF_page structure.  
 *  @return Void.
 */
 
int no = 0;
void PrintText(HPDF_Page page)
{
    char buf[512];
    HPDF_Point pos = HPDF_Page_GetCurrentTextPos (page);

    no++;
    #ifdef __WIN32__
    _snprintf (buf, 512, ".[%d]%0.2f %0.2f", no, pos.x, pos.y);
    #else
    snprintf (buf, 512, ".[%d]%0.2f %0.2f", no, pos.x, pos.y);
    #endif
    HPDF_Page_ShowText(page, buf);
}

/** @brief Generate PDF Name.
 *
 *  This function generates the PDF name based on the naming convention i.e. current date.
 * 
 *  @return char* Name of the PDF file.
 */
static char * generate_pdf_name()
{
	static char pdf_fname[50];
	char *months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char yy[5],dd[3],mm[4];
	time_t c_time;
	struct tm *timeinfo;
	memset(pdf_fname,'\0',50);
	time (&c_time);
	timeinfo = localtime (&c_time);
    	sprintf(yy,"%d",(timeinfo->tm_year+1900));
	sprintf(dd,"%d",timeinfo->tm_mday);
	strcpy(mm,months[timeinfo->tm_mon]);
	strcpy(pdf_fname,mm);
	strcat(pdf_fname, "-");
	strcat(pdf_fname,dd);
	strcat(pdf_fname, "-");
	strcat(pdf_fname,yy);	
	strcat(pdf_fname, "-");
	strcat(pdf_fname, "Validation");
        strcat(pdf_fname, ".pdf");
	
	return pdf_fname;
    
}

/** @brief Generate PDF.
 *
 *  This function generates the PDF file for surrogate gas validation.
 * 
 *  @param g_src[] gas source.
 *  @param f1 parameter of gas values.
 *  @param f2 parameter of gas values.
 *  @param f3 parameter of gas values. 
 *  @param num_points total number of gas source.
 *  @return char* Name of the PDF file.
 */
char* generate_pdf(char *g_src[],float *f1,float *f2,float *f3,int num_points)
{
    HPDF_Doc  pdf;
    HPDF_Page page;
    HPDF_Image image;
    char *fname;
    HPDF_Font font;
    HPDF_REAL page_height;
    HPDF_Rect rect;
    int i;
    char buf1[15];
    int font_l=0,font_t=0;
    short int g_top = 0;	
    //FILE *fp;
    const char* SAMP_TXT = "The quick brown fox jumps over the lazy dog. ";
    HPDF_REAL text_width;// = HPDF_Page_TextWidth ( pdfPage, pText );
    memset(buf1,'\0',15);
    
    fname = generate_pdf_name();
#if DEBUG
        printf("Current date: =%s\n",fname);
#endif
    //Removing the blank spaces from date
    /*i = 0;
    for(; i < strlen(fname); i++)
    {
     if( !isalnum(fname[i]) )
     {
        fname[i] = '-';
     }
    }*/
    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
	return NULL;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return NULL;
    }
#if DEBUG
	printf("pdf file name    fname=%s\n",fname);
#endif
    /* add a new page object. */
    page = HPDF_AddPage (pdf);
    HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    //print_grid  (pdf, page);

    page_height = HPDF_Page_GetHeight (page);

    font = HPDF_GetFont (pdf, "Courier-Bold", NULL);
    HPDF_Page_SetTextLeading (page, 20);

    /* HPDF_TALIGN_LEFT */
    rect.left = 25;
    rect.top = 745;
    rect.right = 200;
    rect.bottom = rect.top - 40;


    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 10);
    font_l = rect.left+45;
    font_t = rect.top + 30;
   /* HPDF_Page_TextOut (page, font_l, font_t, "GAS_TABLE");
    text_width = HPDF_Page_TextWidth ( page, "GAS_TABLE" );
    font_l = font_l+5+(int)text_width;*/
    {
    g_top = font_t;
    HPDF_Page_TextOut (page, font_l, font_t, "GAS_SOURCE"); 
    font = HPDF_GetFont(pdf,"Helvetica", NULL);
    HPDF_Page_SetFontAndSize (page, font, 8);
    for(i=0;i<num_points;i++)
      {
      g_top = g_top - 10;
      HPDF_Page_TextOut (page, font_l, g_top, g_src[i]);	
      }
    }
    {
    g_top = font_t; 
    font = HPDF_GetFont (pdf, "Courier-Bold", NULL);
    HPDF_Page_SetFontAndSize (page, font, 10);
    text_width = HPDF_Page_TextWidth (page, "GAS_SOURCE");
    font_l = font_l+15+(int)text_width;
    HPDF_Page_TextOut (page, font_l, font_t, "Nominal_Methane_PPM");
    font = HPDF_GetFont(pdf,"Helvetica", NULL);
    HPDF_Page_SetFontAndSize (page, font, 8);
    for(i=0;i<num_points;i++)
      {
      g_top = g_top - 10;
      sprintf(buf1,"%f",f2[i]);
      HPDF_Page_TextOut (page, font_l, g_top, buf1);	
      }
    }
    {
    g_top = font_t; 
    font = HPDF_GetFont (pdf, "Courier-Bold", NULL);
    HPDF_Page_SetFontAndSize (page, font, 10);
    text_width = HPDF_Page_TextWidth ( page, "Nominal_Methane_PPM" );
    font_l = font_l+15+(int)text_width;
    HPDF_Page_TextOut (page, font_l, font_t, "Mean_Observed_Concentration");
    font = HPDF_GetFont(pdf,"Helvetica", NULL);
    HPDF_Page_SetFontAndSize (page, font, 8);
    for(i=0;i<num_points;i++)
      {
      g_top = g_top - 10;
      sprintf(buf1,"%f",f1[i]);
      HPDF_Page_TextOut (page, font_l, g_top, buf1);	
      }
    }
    {
    g_top = font_t;
    font = HPDF_GetFont (pdf, "Courier-Bold", NULL);
    HPDF_Page_SetFontAndSize (page, font, 10);
    text_width = HPDF_Page_TextWidth ( page, "Mean_Observed_Concentration" );
    font_l = font_l+15+(int)text_width;
    HPDF_Page_TextOut (page, font_l, font_t, "SD");
    font = HPDF_GetFont(pdf,"Helvetica", NULL);
    HPDF_Page_SetFontAndSize (page, font, 8);
    for(i=0;i<num_points;i++)
      {
      g_top = g_top - 10;
      sprintf(buf1,"%f",f3[i]);
      HPDF_Page_TextOut (page, font_l, g_top, buf1);	
      }
    }

    HPDF_Page_EndText (page);

    /* HPDF_TALIGN_RIGTH */
    rect.left = 40;
    rect.right = 100;

    image = HPDF_LoadPngImageFromFile (pdf,"xy_graph.png");

    /* Draw image to the canvas. */
    HPDF_Page_DrawImage (page, image, 60, 350, HPDF_Image_GetWidth (image),
               HPDF_Image_GetHeight (image));

    /* save the document to a file */
    HPDF_SaveToFile (pdf, fname);

    /* clean up */
    HPDF_Free (pdf);

    return fname;
}
