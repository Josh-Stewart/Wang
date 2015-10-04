/* bresenham circle v1
   x + y is used as an origin

   plotpoints() function ripped from allegro do_circle() source
   the extra if statements avoid overdraw
   
   -JS
*/

#include "allegro.h"
#include "globals.h"

void plotpoints(BITMAP *bmp, int x, int y, int cx, int cy, int color);

void circlebres(BITMAP *bmp, int x, int y, int radius, int color)
{
   int cx = 0;
   int cy = radius;
   int p = 3 - 2 * radius;

   while (cx < cy) {


      plotpoints(bmp,x,y,cx,cy,color);
      
      if (p < 0)

         p = p + 4 * cx + 6;
         
      else { 

         p = p + 4 * (cx - cy) + 10;
         cy--;
      }
      
      cx++;

   } //end while

   if (x == y)
      plotpoints(bmp,x,y,cx,cy,color);
      
} //end circlebres

void plotpoints(BITMAP *bmp, int x, int y, int cx, int cy, int color)
{
 //_putpixel32 speed?
 if (!(x+cx < GUIWIDTH || x+cx >= XMAX || y+cy < 0 || y+cy >= YMAX)) //clipcheck
 _putpixel24(bmp, x+cx, y+cy, color);

 if (cx)
    if (!(x-cx < GUIWIDTH || x-cx >= XMAX || y+cy < 0 || y+cy >= YMAX)) //clipcheck
    _putpixel24(bmp, x-cx, y+cy, color);

 if (cy)
    if (!(x+cx < GUIWIDTH || x+cx >= XMAX || y-cy < 0 || y-cy >= YMAX)) //clipcheck
    _putpixel24(bmp, x+cx, y-cy, color);

 if ((cx) && (cy))
    if (!(x-cx < GUIWIDTH || x-cx >= XMAX || y-cy < 0 || y-cy >= YMAX)) //clipcheck
    _putpixel24(bmp, x-cx, y-cy, color);

 if (cx != cy) {
    if (!(x+cy < GUIWIDTH || x+cy >= XMAX || y+cx < 0 || y+cx >= YMAX)) //clipcheck
    _putpixel24(bmp, x+cy, y+cx, color);

    if (cx)
       if (!(x+cy < GUIWIDTH || x+cy >= XMAX || y-cx < 0 || y-cx >= YMAX)) //clipcheck
       _putpixel24(bmp, x+cy, y-cx, color);

    if (cy)
       if (!(x-cy < GUIWIDTH || x-cy >= XMAX || y+cx < 0 || y+cx >= YMAX)) //clipcheck
       _putpixel24(bmp, x-cy, y+cx, color);

    if (cx && cy)
       if (!(x-cy < GUIWIDTH || x-cy >= XMAX || y-cx < 0 || y-cx >= YMAX)) //clipcheck
       _putpixel24(bmp, x-cy, y-cx, color);

 }
}
