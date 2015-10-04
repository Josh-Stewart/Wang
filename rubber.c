/* 
   bresenham ruberline (now with linked list saving ;)
   hacked from my mousedragv3.c :)
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <allegro.h>
#include "linebres.h"
#include "circlebres.h"
#include "list.h"
#include "globals.h"

inline int calc_radius(int x1, int y1, int x2, int y2)
{

    return (int) (sqrt
		  ((double)
		   ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))));
}

void rsave(BITMAP * bmp, int x, int y, int d)
{
    /* save pixel information into array */
    // clipping
    if (x < GUIWIDTH || x >= XMAX || y < 0 || y >= YMAX)
	return;
    savedpixels[pos_sp].x = x;
    savedpixels[pos_sp].y = y;
    savedpixels[pos_sp].color = _getpixel24(bmp, x, y);
    pos_sp++;
    /* Doing this will remove the need to call circle again */
    _putpixel24(bmp, x, y, d);
}

void rrestore(BITMAP * bmp)
{
    int i;
    
    pixel *ptr = (pixel *) savedpixels;

    for (i = 0; i < pos_sp; i++, ptr++)	/* from savedpixels[0] -> savedpixels[ii]
					   * is putpixel too slow? maybe this is causing flickering? */
	_putpixel24(bmp, ptr->x, ptr->y, ptr->color);

    pos_sp = 0;			/* all done.. lets start on a new circle */
}

void rubberline(List thelist, int color)
{
    int x1, x2, y1, y2;
    int x3, y3;

    Line_Type linedata;

    while (!(mouse_b & 2)) {  //quick addin

    if (mouse_b & 1) {

       x3 = x1 = mouse_x;
       y3 = y1 = mouse_y;
       scare_mouse();

       /* draw circle while waiting for mouse release */
       while (mouse_b & 1) {
             x2 = mouse_x;
             y2 = mouse_y;

             if (x2 != x3 || y2 != y3) {

                rrestore(screen);//first time through ii=0
                do_line(screen,x1,y1,x2,y2, color, rsave);
                x3 = x2;
                y3 = y2;
             }

       }

       rrestore(screen);
       x2 = mouse_x;
       y2 = mouse_y;
       linebres(x1,y1,x2,y2,color);

       linedata.p1.x = x1;
       linedata.p1.y = y1;
       linedata.p2.x = x2;
       linedata.p2.y = y2;
       linedata.color = color;
       linedata.width = 1;

       AddLine(thelist,linedata);

       unscare_mouse();

    }
    }// end whileaddin

}

void rubbercircle(List thelist, int color)
{
    int x1, x2, y1, y2;
    int radius;
    int x3, y3;

    Circle_Type circledata;

    while (!(mouse_b & 2)) {

    if (mouse_b & 1) {
       x3 = x1 = mouse_x;
       y3 = y1 = mouse_y;
       scare_mouse();

	    /* draw circle while waiting for mouse release */
	    while (mouse_b & 1) {
		x2 = mouse_x;
		y2 = mouse_y;

		if (x2 != x3 || y2 != y3) {

		    rrestore(screen);//first time through ii=0
		    radius = calc_radius(x1, y1, x2, y2);
		    /* save/draw/wait/restore */
		    do_circle(screen, x1, y1, radius, color, rsave);
		    x3 = x2;
		    y3 = y2;
		}
	    }


	    rrestore(screen);
	    x2 = mouse_x;
	    y2 = mouse_y;
	    radius = calc_radius(x1, y1, x2, y2);
	    circlebres(screen, x1, y1, radius, color);

            circledata.p1.x = x1;
            circledata.p1.y = y1;
            circledata.radius = radius;
            circledata.color = color;
            circledata.filled = FALSE;
            circledata.fill_color = 0;
            circledata.width = 1;

            AddCircle(thelist, circledata);
            
	    unscare_mouse();
	}

    } //end while

}



int rubberpolygon(List thelist, int color)
{
 int x1, x2, y1, y2;
 int x3, y3;

 bool firstrun = TRUE;

 PList polylist;
 Poly_Type polydata;

 polylist = PCreateList(); //make a point list

 while (!(mouse_b & 2)) {

       if (mouse_b & 1) {
          if (firstrun){
             x3 = x1 = mouse_x;
             y3 = y1 = mouse_y;
             PAddNode(polylist,x1,y1);
             scare_mouse();
          }
          else {
               x3 = x1 = x2;
               y3 = y1 = y2;
          }

          /* draw line while waiting for mouse release */
          while (mouse_b & 1) {
                x2 = mouse_x;
                y2 = mouse_y;

                if (x2 != x3 || y2 != y3) {
                   rrestore(screen);//first time through ii=0
                   do_line(screen,x1,y1,x2,y2,color, rsave);
                   x3 = x2;
                   y3 = y2;
                }
          }
          /* on m1 release */
          rrestore(screen);
          x2 = mouse_x; // get current xy
          y2 = mouse_y;
          linebres(x1,y1,x2,y2,color);

          PAddNode(polylist,x2,y2); //add second point

          firstrun = FALSE;

       }
 
 }//while !mouse 2
 linebres(polylist->last->ppoint.x,polylist->last->ppoint.y,
          polylist->first->ppoint.x,polylist->first->ppoint.y,color);
 //textprintf(screen,font,1,1,color,"%d",polylist->length);

 polydata.theplist = polylist;
 polydata.color = color;
 polydata.filled = FALSE;
 polydata.fill_color = 0;
 polydata.width = 1;
 
 AddPolygon(thelist, polydata);

 //DisposePList(polylist);//oops ;)
 unscare_mouse();
 return 0;
}

void rubberrectangle(List thelist, int color)
{
    int x1, x2, y1, y2;
    int x3, y3;

    Poly_Type polydata;
    PList polylist;

    polylist = PCreateList();

    while (!(mouse_b & 2)) {  //quick addin

    if (mouse_b & 1) {

       x3 = x1 = mouse_x;
       y3 = y1 = mouse_y;
       scare_mouse();

       /* draw box while waiting for mouse release */
       while (mouse_b & 1) {
             x2 = mouse_x;
             y2 = mouse_y;
             
             if (x2 != x3 || y2 != y3) {

                rrestore(screen);//first time through ii=0
                if (x1 < x2) {
                   do_line(screen,x1+1,y1,x2-1,y1, color, rsave);
                   do_line(screen,x2,y1+1,x2,y2-1, color, rsave);
                   do_line(screen,x2-1,y2,x1+1,y2, color, rsave);
                   do_line(screen,x1,y2-1,x1,y1+1, color, rsave);
                }
                if (x2 < x1) {
                   do_line(screen,x1-1,y1,x2+1,y1, color, rsave);
                   do_line(screen,x2,y1-1,x2,y2+1, color, rsave);
                   do_line(screen,x2+1,y2,x1-1,y2, color, rsave);
                   do_line(screen,x1,y2+1,x1,y1-1, color, rsave);
                }
                x3 = x2;
                y3 = y2;
             }
       }/* end while */
        rrestore(screen);
        x2 = mouse_x;
        y2 = mouse_y;
        //draw final points
        linebres(x1,y1,x2,y1,color);
        linebres(x2,y1,x2,y2,color);
        linebres(x2,y2,x1,y2,color);
        linebres(x1,y2,x1,y1,color);

        //add points to plist
        PAddNode(polylist,x1,y1);
        PAddNode(polylist,x2,y1);
        PAddNode(polylist,x2,y2);
        PAddNode(polylist,x1,y2);

        polydata.theplist = polylist;
        polydata.color = color;
        polydata.filled = FALSE;
        polydata.fill_color = 0;
        polydata.width = 1;
 
        AddPolygon(thelist, polydata);

        polylist = PCreateList(); //leak?

        unscare_mouse();
      }
    }// end whileaddin
}

