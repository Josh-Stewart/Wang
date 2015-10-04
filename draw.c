
#include "allegro.h"
#include "draw.h"
#include "list.h"
#include "globals.h"
#include "linebres.h"
#include "circlebres.h"

void DrawList(List thelist)
{
 // traverse the list from last to first.. drawing each object
 NodeLink currnode;
 int color;
 thelist->cursor = thelist->last;

 scare_mouse();

 while (thelist->cursor != NULL) {

       currnode = thelist->cursor;

       if (currnode->Object == LINE) {
          linebres(currnode->obj_data.line.p1.x,
                   currnode->obj_data.line.p1.y,
                   currnode->obj_data.line.p2.x,
                   currnode->obj_data.line.p2.y,
                   currnode->obj_data.line.color);
       }
       else {
            if (currnode->Object == CIRCLE) {
               circlebres(screen,
                          currnode->obj_data.circle.p1.x,
                          currnode->obj_data.circle.p1.y,
                          currnode->obj_data.circle.radius,
                          currnode->obj_data.circle.color);
            }
            else {
                if (currnode->Object == POLYGON) {
                   color = currnode->obj_data.polygon.color;
                   drawpoly(currnode,color);
                }
                else {
                     if (currnode->Object == TEXT) {

                        textout(screen,font,currnode->obj_data.text.data,
                                         currnode->obj_data.text.p1.x,
                                         currnode->obj_data.text.p1.y,
                                         currnode->obj_data.text.color);
                     }
                }

            }
            

       }

       thelist->cursor = thelist->cursor->prev; //mode cursor back a node

 }

 unscare_mouse(); 

}

void drawpoly(NodeLink currnode, int color)
{
 PList polylist;
 polylist = currnode->obj_data.polygon.theplist;

 polylist->cursor = polylist->first;
 
 while (polylist->cursor->next != NULL) {
       linebres(polylist->cursor->ppoint.x,
                polylist->cursor->ppoint.y,
                polylist->cursor->next->ppoint.x,
                polylist->cursor->next->ppoint.y,
                color);

       polylist->cursor = polylist->cursor->next;
 }

 linebres(polylist->last->ppoint.x,
          polylist->last->ppoint.y,
          polylist->first->ppoint.x,
          polylist->first->ppoint.y,
          color);
}



       
