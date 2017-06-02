/*---------------------- Copyright (c) Picarro, 2017 -------------------
 *                                 Source code
 * This software is the property of Picarro and should be considered and treated
 * as proprietary information.  Refer to the "Source Code License Agreement"
 *----------------------------------------------------------------------------
*/

/** @brief plot grap program
 *  @file plot_graph.c
 *  @author Aricent
 *  @date Apr 2017
 *  @version 1.0
 *  
 *  This file creates the X-Y linear scatter graph in PNG file using GNUplot utility.
 *  
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "plot_graph.h"
#include "gnuplot_interface.h"
#include "debug.h"

/** @brief Plot graph.
 *
 *  This function creates the X-Y linear scatter graph in PNG file using GNUplot utility.
 * 
 *  @param f1 Data point for X axis.
 *  @param f2 Data point for Y axis.
 *  @param num_points Total number of points.
 *  @return int return 0 on success.
 */
int plot_graph(float *f1, float *f2,int num_points) 
{
    gnuplot_ctrl    *   graph_handle;
    double              x[num_points] ;
    double              y[num_points] ;
    int                 i ;
    /*
     * Initialize the gnuplot handle
     */
#if DEBUG
    print("*** example of gnuplot control through C ***\n") ;
#endif
    graph_handle = gnuplot_init();
#if DEBUG
    print("*** user-defined lists of points\n");
#endif
    for (i=0 ; i<num_points ; i++) {
        x[i] = (double)f1[i] ;
        y[i] = (double)f2[i];
    }
    gnuplot_resetplot(graph_handle) ;

    //gnuplot_cmd(graph_handle,"set style line 8 lt 2 lw 2 pt 10 ps 5");
    //gnuplot_cmd(graph_handle,"set style line 1 linetype 2 pointtype 13 linewidth 1");
    gnuplot_setstyle(graph_handle, "linespoints");
    gnuplot_cmd(graph_handle,"set grid xtics ytics ls 3 lw 1 lc rgb \"gray\"");
    gnuplot_cmd(graph_handle,"set xlabel \"Nominal CH4 ppm\"");
    gnuplot_cmd(graph_handle,"set ylabel \"Observed CH4 ppm\"");
    gnuplot_cmd(graph_handle,"set title \"Surrogate Gas Validation\"");
    gnuplot_cmd(graph_handle, "set terminal png font arial 14 size 450,300");
    gnuplot_cmd(graph_handle, "set output \"xy_graph.png\"");
    gnuplot_plot_xy(graph_handle,x,y,num_points, "");
    sleep(2) ;
    
    /*
     * close gnuplot handles
     */
#if DEBUG
    print("\n\n") ;
    print("*** end of gnuplot example\n") ;
#endif
    gnuplot_close(graph_handle) ;
    return 0 ;
}
