/*
   Josh Stewart
   CSD2341 - Computer Graphics Programming
   Week 01 - Bresenhams Line

   Note: this is a generic version, the text version only
         worked for some vectors.

   Credit to Mark Feldman who wrote the bresenham line tute
   and a generic algorithm in pascal...
   both which gave me a greater understanding of bresenham line

   You need DJGPP and Allegro to compile this
*/

#include "allegro.h"
#include "globals.h"

void linebres(int x1, int y1, int x2, int y2, int color)
{
 int x,y;
 int d,numpixels;
 int dinc1,dinc2,xinc1,xinc2,yinc1,yinc2;
 int i;

 int deltax = ABS(x2 - x1);
 int deltay = ABS(y2 - y1);

 if (deltax >= deltay) {

    numpixels = deltax;
    d = (2 * deltay) - deltax;
    dinc1 = 2 * deltay;
    dinc2 = 2 * (deltay - deltax);
    xinc1 = 1;
    xinc2 = 1;
    yinc1 = 0;
    yinc2 = 1;
 }
 else {
 
      numpixels = deltay;
      d = (2 * deltax) - deltay;
      dinc1 = 2 * deltax;
      dinc2 = 2 * (deltax - deltay);
      xinc1 = 0;
      xinc2 = 1;
      yinc1 = 1;
      yinc2 = 1;
 }

 if (x1 > x2) {
    xinc1 = -xinc1;
    xinc2 = -xinc2;
 }
 if (y1 > y2) {
    yinc1 = -yinc1;
    yinc2 = -yinc2;
 }

 x = x1;
 y = y1;

 for (i=0;i<numpixels;i++) {

     if (!(x < GUIWIDTH || x >= XMAX || y < 0 || y >= YMAX)) //clipcheck
        _putpixel24(screen,x,y,color);
     
     if (d < 0) {
        d += dinc1;
        x += xinc1;
        y += yinc1;
     }
     else {
          d += dinc2;
          x += xinc2;
          y += yinc2;
     }
 }

}




