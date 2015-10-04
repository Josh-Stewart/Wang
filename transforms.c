
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "allegro.h"
#include "globals.h"
#include "list.h"
#include "transforms.h"

/* takes a polylist and does a move on it.
 */
void plistmove(PList polylist, Point new_xy)
{
 int n;
 Point *p;
 Point centroid;
 Point xy_offset;

 n = polylist->length; //number of elemtents
 p = calloc(n, sizeof(Point));

 
 // traverse the list from last to first to make the Point array
 polylist->cursor = polylist->last;

 while (polylist->cursor != NULL) {

       p[n-1] = polylist->cursor->ppoint;

       polylist->cursor = polylist->cursor->prev; //move cursor back a node
       n--;
 }
 //reset n
 n = polylist->length;
 
 //calc centroid
 centroid = calc_centroid(p,n);

 //calc xy_offset
 xy_offset.x = new_xy.x - centroid.x;
 xy_offset.y = new_xy.y - centroid.y;

 //pass point array to move_n_poloy
 move_n_poly(p,n,centroid,xy_offset);

 //take values from point array and put them back into polylist
 polylist->cursor = polylist->last;  //reset cursor

 while (polylist->cursor != NULL) {

       polylist->cursor->ppoint = p[n-1];

       polylist->cursor = polylist->cursor->prev; //move cursor back a node
       n--;
 }

 //cfree(p); //djgpp
 free(p);

}

/* takes a polylist and does a rotate or scale on it depending on (*proc)
 */
Point plistscalerotate(PList polylist, Point centroid, double scalerotate, void (*proc)(Point *,int ,Point ,double ))
{
 int n;
 Point *p;

 n = polylist->length; //number of elemtents
 p = calloc(n, sizeof(Point));

 
 // traverse the list from last to first to make the Point array
 polylist->cursor = polylist->last;

 while (polylist->cursor != NULL) {

       p[n-1] = polylist->cursor->ppoint;

       polylist->cursor = polylist->cursor->prev; //move cursor back a node
       n--;
 }
 //reset n
 n = polylist->length;
 
 //calc centroid
 if (centroid.x == 1)
    centroid = calc_centroid(p,n);

 //pass point array to proc()
 proc(p, n, centroid, scalerotate);

 //take values from point array and put them back into polylist
 polylist->cursor = polylist->last;  //reset cursor

 while (polylist->cursor != NULL) {

       polylist->cursor->ppoint = p[n-1];

       polylist->cursor = polylist->cursor->prev; //move cursor back a node
       n--;
 }

 //cfree(p); //djgpp
 free(p); //leaks?


 return centroid;

}


void rotate(double pnt[], double radian)
{

 int i;
 double rotate[3][3];
 double newpoint[3];

 //fill trans - rotation 1 degree (shown in radians)
 rotate[0][0] = cos(radian);//17453292);
 rotate[0][1] = -(sin(radian));//17453292));
 rotate[0][2] = 0;
 rotate[1][0] = sin(radian);//17453292);
 rotate[1][1] = cos(radian);//17453292);
 rotate[1][2] = 0;
 rotate[2][0] = 0;
 rotate[2][1] = 0;
 rotate[2][2] = 1;

 for (i=0;i<=2;i++)
     newpoint[i] = (rotate[i][0] * pnt[0])+(rotate[i][1] * pnt[1])+(rotate[i][2] * pnt[2]);

 for (i=0;i<=2;i++)
     pnt[i] = newpoint[i];


}

void scale(double pnt[], double sfactor)
{

 int i;
 double scale[3][3];
 double newpoint[3];

 //fill trans - scale x0.9
 scale[0][0] = sfactor; //Sx
 scale[0][1] = 0;
 scale[0][2] = 0;
 scale[1][0] = 0;
 scale[1][1] = sfactor; //Sy
 scale[1][2] = 0;
 scale[2][0] = 0;
 scale[2][1] = 0;
 scale[2][2] = 1;

 for (i=0;i<=2;i++)
     newpoint[i] = (scale[i][0] * pnt[0])+(scale[i][1] * pnt[1])+(scale[i][2] * pnt[2]);

 for (i=0;i<=2;i++)
     pnt[i] = newpoint[i];
}


void move(double pnt[], Point xy_offset)
{

 int i;
 double scale[3][3];
 double newpoint[3];

 //fill trans - move
 scale[0][0] = 1;
 scale[0][1] = 0;
 scale[0][2] = xy_offset.x; //Tx
 scale[1][0] = 0;
 scale[1][1] = 1;
 scale[1][2] = xy_offset.y; //Ty
 scale[2][0] = 0;
 scale[2][1] = 0;
 scale[2][2] = 1;

 for (i=0;i<=2;i++)
     newpoint[i] = (scale[i][0] * pnt[0])+(scale[i][1] * pnt[1])+(scale[i][2] * pnt[2]);

 for (i=0;i<=2;i++)
     pnt[i] = newpoint[i];
}


void rotate_n_poly(Point p[], int n, Point centroid, double angle)
{
 int i;
 double pnt[3];
 
 for (i=0;i<n;i++) {
     //load point
     pnt[0] = p[i].x - centroid.x;
     pnt[1] = p[i].y - centroid.y;
     pnt[2] = 1;
     //send to rotate to get next point
     rotate(pnt,angle);
     //unload point
     p[i].x = pnt[0] + centroid.x;
     p[i].y = pnt[1] + centroid.y;
 }

}

void scale_n_poly(Point p[], int n, Point centroid, double sfactor)
{
 int i;
 double pnt[3];

 for (i=0;i<n;i++) {
     //load point
     pnt[0] = p[i].x - centroid.x;
     pnt[1] = p[i].y - centroid.y;
     pnt[2] = 1;
     //send to rotate to get next point
     scale(pnt,sfactor);
     //unload point
     p[i].x = pnt[0] + centroid.x;
     p[i].y = pnt[1] + centroid.y;
 }

}

void move_n_poly(Point p[], int n, Point centroid, Point xy_offset)
{
 int i;
 double pnt[3];

 for (i=0;i<n;i++) {
     //load point
     pnt[0] = p[i].x - centroid.x;
     pnt[1] = p[i].y - centroid.y;
     pnt[2] = 1;
     //send to rotate to get next point
     move(pnt,xy_offset);
     //unload point
     p[i].x = pnt[0] + centroid.x;
     p[i].y = pnt[1] + centroid.y;
 }

}


Point calc_centroid(Point p[], int n)
{

 int i;
 Point centroid;

 // used for polygons in all areas, where all points are already known
 // the origin (centroid - worked out from a N screen points)
 centroid.x = 0;
 centroid.y = 0;
 
 for (i=0;i<n;i++)
     centroid.x += p[i].x;

 centroid.x /= n;

 for (i=0;i<n;i++)
     centroid.y += p[i].y;

 centroid.y /= n;

 return centroid;

}

//JS