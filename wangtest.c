#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro.h>
//#include <math.h>      //for calc_radius
#include "globals.h"
#include "list.h"
#include "linebres.h"    //these are for draw all
#include "circlebres.h"  //
#include "rubber.h"      //this is for the rubberbanded objects and saving to the ds
#include "transforms.h"
#include "draw.h"

//defines for clearhack
#define PIXEL_PTR              unsigned char*
#define OFFSET_PIXEL_PTR(p,x)  ((PIXEL_PTR) (p) + 3 * (x))
#define INC_PIXEL_PTR(p)       ((p) += 3)
#define PUT_PIXEL(p,c)         bmp_write24((unsigned long) (p), (c))

NodeLink FindSelection(List thelist, NodeLink lastfound, int x, int y);
void SelectMode(List thelist);
void MoveObject(NodeLink thenode);
void Save(List thelist);
void text(List thelist, int color);
void setnodecolor(List thelist, int color);

//dialog operations
void SetDialogColors(void);
void redrawmenu();
int update_color(void *dp3, int d2);

//custom dialog procedures
int line_proc(int msg, DIALOG *wang, int c);
int polyline_proc(int msg, DIALOG *wang, int c);
int circle_proc(int msg, DIALOG *wang, int c);
int rect_proc(int msg, DIALOG *wang, int c);
int rotate_proc(int msg, DIALOG *wang, int c);
int scale_proc(int msg, DIALOG *wang, int c);
int selectmode_proc(int msg, DIALOG *wang, int c);
int text_proc(int msg, DIALOG *wang, int c);
int colchange_proc(int msg, DIALOG *wang, int c);
int copynode_proc(int msg, DIALOG *wang, int c);
int save_proc(int msg, DIALOG *wang, int c);
int exit_proc(int msg, DIALOG *wang, int c);

void clearhack(BITMAP *dst, int color);

//allegro optimisations
BEGIN_COLOR_DEPTH_LIST
   COLOR_DEPTH_24
END_COLOR_DEPTH_LIST

BEGIN_DIGI_DRIVER_LIST

END_DIGI_DRIVER_LIST

BEGIN_MIDI_DRIVER_LIST

END_MIDI_DRIVER_LIST

BEGIN_JOYSTICK_DRIVER_LIST

END_JOYSTICK_DRIVER_LIST
// end allegro optimisations

char text_string[42] = "Insert Text Here..";

DIALOG wang_dialog[] = {
	  /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)  (dp)  (dp2)         (dp3) */
/* 0 */ { d_box_proc  ,         0,    0,    64,   570,  255,  0,    0,    0,       0,   0,    NULL },
/* 1 */	{ selectmode_proc,      4 ,   6 ,   24 ,  24 ,  251,  46,   0 ,	  D_EXIT , 0 ,	0 ,  "S" },
/* 2 */	{ d_button_proc ,       34 ,  6 ,   24 ,  24 ,  251,  46,   0 ,	  D_EXIT , 0 ,	0 ,  "MS" },
/* 3 */	{ line_proc     ,       4 ,   36 ,  24 ,  24 ,  251,  46,   0 ,	  D_EXIT,  0 ,	0 ,  "L" },
/* 4 */	{ circle_proc   ,       34 ,  36 ,  24 ,  24 ,  251,  46,   0 ,	  D_EXIT,  0 ,  0 ,  "C" },
/* 5 */	{ polyline_proc ,       4 ,   66 ,  24 ,  24 ,  251,  46,   0 ,	  D_EXIT,  0 ,  0 ,  "P" },
/* 6 */	{ rect_proc     ,       34 ,  66 ,  24 ,  24 ,  251,  46,   0 ,	  D_EXIT , 0 ,	0 ,  "R" },
/* 7 */	{ scale_proc    ,       4 ,   96 ,  24 ,  24 ,  251,  46,   0 ,	  D_DISABLED , 0 ,	0 ,  "Sc" },
/* 8 */	{ rotate_proc   ,       34 ,  96 ,  24 ,  24 ,  251,  46,   0 ,	  D_DISABLED , 0 ,	0 ,  "Ro" },
/* 9 */	{ text_proc     ,       34 ,  126 , 24 ,  24 ,  251,  46,   0 ,	  D_EXIT,  0 ,	0 ,  "T" },
/* 10*/	{ colchange_proc,       34,   186 , 24 ,  24 ,  251,  46,   0 ,	  D_EXIT , 0 ,	0 ,  "Col" },
/* 11 */{ save_proc     ,	4 ,   156 , 24 ,  24 ,  251,  46,   0 ,   D_EXIT , 0 ,  0 ,  "Save" },
/* 12 */{ exit_proc     ,	34 ,  156 , 24 ,  24 ,  251,  46,   0 ,	  D_EXIT , 0 ,  0 ,  "Exit" },
/* 13 */{ d_box_proc,           4,    186,  24,   24,    0,  254,    0,    0,       0,  0,    NULL },
/* 14 */{ d_slider_proc,        4,    216,  16,   64,    1,    0,    0,    0,      255, 255,  NULL, update_color, NULL },
/* 15 */{ d_slider_proc,        24,   216,  16,   64,    2,    0,    0,    0,      255, 255,  NULL, update_color, NULL },
/* 16 */{ d_slider_proc,        44,   216,  16,   64,    4,    0,    0,    0,      255, 255,  NULL, update_color, NULL },
/* 17 */{ d_edit_proc ,         4,    570,  200,  30,   251,   0,    0,    0,      40,  0,  text_string},
/* 18 */{ copynode_proc,        4 ,   126 , 24 ,  24 ,  251,  46,   0 ,	  D_EXIT , 0 ,	0 ,  "Copy" },
{ NULL,              0,    0,    0,    0,    0,    0,    0,    0,       0,    0,    NULL }
 };

int main()
{
 List thelist;// the linked list

 allegro_init();
 install_keyboard();
 install_mouse();
 install_timer();

 set_color_depth(COLORDEPTH);
 if ((set_gfx_mode(GFX_AUTODETECT, XMAX, YMAX, 0, 0)) != 0) {
      allegro_exit();
      fprintf(stderr, "Couldn't set mode: %s", allegro_error);
      exit(1);
 }
 thelist = CreateList();

 SetDialogColors();
 //put list pointers in all the buttons
 wang_dialog[B_L].dp2 = thelist;
 wang_dialog[B_C].dp2 = thelist;
 wang_dialog[B_P].dp2 = thelist;
 wang_dialog[B_S].dp2 = thelist;
 wang_dialog[B_R].dp2 = thelist;
 wang_dialog[B_Ro].dp2 = thelist;
 wang_dialog[B_Sc].dp2 = thelist;
 wang_dialog[B_Sa].dp2 = thelist;
 wang_dialog[B_Ex].dp2 = thelist;
 wang_dialog[B_T].dp2 = thelist;
 wang_dialog[B_Col].dp2 = thelist;
 wang_dialog[B_Cop].dp2 = thelist;
 
 do_dialog(wang_dialog, -1);


 DisposeList(thelist);  //clean up

 return 0;
}

void SelectMode(List thelist)
{
 int x,y;
 
 bool mouse1pressed;
 NodeLink thenode = NULL;
 NodeLink lastfound = thelist->last;

 while(!(mouse_b & 2)) {
 
       while (mouse_b & 1) {
             x = mouse_x;
             y = mouse_y;
             mouse1pressed = TRUE;
       }

       if (mouse1pressed) {
          
          thenode = FindSelection(thelist, lastfound, x, y);
          if (thenode == NULL)   //search failed
             lastfound = thelist->last;
          else
             lastfound = thenode->prev;  //next click make sure we start looking at the next node

          if ((lastfound == NULL) && (thenode != NULL)) { //if we are at the first node
             lastfound = thelist->last; // make sure next search starts at end
            // thenode = FindSelection(thelist, lastfound->prev,x,y);
          }

          mouse1pressed = FALSE;

       } /* if */
       if (key[KEY_M]) {
          MoveObject(thenode);
          clearhack(screen,0);
          DrawList(thelist);
          //textout(screen,font,"test",100,100,makecol24(255,255,0));
          //redrawmenu();
       }
       
       if (key[KEY_DEL]) {
          if (thenode != NULL) {
             thelist->cursor = thenode;
             DeleteNode(thelist);
             thenode = NULL;
             clearhack(screen,0);
             DrawList(thelist);
             //redrawmenu();
             //return;
          }
       }
       
 } /* while */
 thelist->cursor = thenode; //set cursor to the selected node of future manipulation.
 if (thenode != NULL) {
    wang_dialog[B_Sc].flags = D_EXIT;
    wang_dialog[B_Ro].flags = D_EXIT;
 }
 else {
      wang_dialog[B_Sc].flags = D_DISABLED;
      wang_dialog[B_Ro].flags = D_DISABLED;
 }
}

/* takes a node and loads it into a poly move function
 * depending on what object type it is..
 */
void MoveObject(NodeLink thenode)
{
 Point centroid;
 Point xy_offset;
 Point p[2]; //only for lines
 int n;      //number of points in object (lines = 2)

             if (thenode != NULL) {

                if (thenode->Object == LINE) {
                   n = 2;

                   //load nodes points into p[]
                   p[0].x = thenode->obj_data.line.p1.x;
                   p[0].y = thenode->obj_data.line.p1.y;
                   p[1].x = thenode->obj_data.line.p2.x;
                   p[1].y = thenode->obj_data.line.p2.y;

                   centroid = calc_centroid(p,n);

                   xy_offset.x = mouse_x - centroid.x;
                   xy_offset.y = mouse_y - centroid.y;

                   move_n_poly(p,n,centroid,xy_offset);

                   //unload from p into node
                   thenode->obj_data.line.p1.x = p[0].x;
                   thenode->obj_data.line.p1.y = p[0].y;
                   thenode->obj_data.line.p2.x = p[1].x;
                   thenode->obj_data.line.p2.y = p[1].y;
                }
                else {
                    if (thenode->Object == CIRCLE) {
                       n = 1;
                       p[0].x = thenode->obj_data.circle.p1.x;
                       p[0].y = thenode->obj_data.circle.p1.y;

                       centroid = calc_centroid(p,n);

                       xy_offset.x = mouse_x - centroid.x;
                       xy_offset.y = mouse_y - centroid.y;

                       move_n_poly(p,n,centroid,xy_offset);

                       thenode->obj_data.circle.p1.x = p[0].x;
                       thenode->obj_data.circle.p1.y = p[0].y;
                    }
                    else {
                         if (thenode->Object == POLYGON) {
                            xy_offset.x = mouse_x; // loads current mouse position
                            xy_offset.y = mouse_y; // to be processed by plistmove

                            plistmove(thenode->obj_data.polygon.theplist,
                                      xy_offset);
                         }/* if */
                         else {
                              if (thenode->Object == TEXT) {
                                 thenode->obj_data.text.p1.x = mouse_x;
                                 thenode->obj_data.text.p1.y = mouse_y;
                              }
                         }
                    }/* else */



                }/* else */

             }

}

/* takes a point on the screen and search's the list backwards
 * from lastnode found until it finds a node or null
 * colors the selected object red
 * return the node pointer
 */
NodeLink FindSelection(List thelist, NodeLink lastfound, int x, int y)
{
 int SA; //dynamic selection area envelope
 Point centroid; //might need centroid calculator for polys

 NodeLink currnode;

 //clear off red bits for debugging
 //clear(screen);
 DrawList(thelist);

 thelist->cursor = lastfound;
    
    if (thelist->length != 0) {

          while (thelist->cursor != NULL) {

                currnode = thelist->cursor;

                if (currnode->Object == LINE) {
                   //textout(screen,font,"poly not selected",1,1,makecol24(255,0,0));
                   centroid.x = (currnode->obj_data.line.p1.x + currnode->obj_data.line.p2.x) / 2;
                   centroid.y = (currnode->obj_data.line.p1.y + currnode->obj_data.line.p2.y) / 2;

                   //calculate the length of half the line, given the center and an end point
                   SA = calc_radius(centroid.x,centroid.y,currnode->obj_data.line.p2.x,currnode->obj_data.line.p2.y);

                   if ((x > (centroid.x-SA)) && (x < (centroid.x+SA))) {
                      if ((y > (centroid.y-SA)) && (y < (centroid.y+SA))) {
                         scare_mouse();
                         linebres(currnode->obj_data.line.p1.x,currnode->obj_data.line.p1.y,currnode->obj_data.line.p2.x,currnode->obj_data.line.p2.y,makecol24(255,0,0));
                         unscare_mouse();
                         return currnode;
                      }
                   }
                }
                else {
                    if (currnode->Object == CIRCLE) {
                       //textout(screen,font,"poly not selected",1,1,makecol24(255,0,0));
                       //we already know the centroid and the radius ;)

                       centroid.x = currnode->obj_data.circle.p1.x;
                       centroid.y = currnode->obj_data.circle.p1.y;
                       SA = currnode->obj_data.circle.radius;

                       if ((x > (centroid.x-SA)) && (x < (centroid.x+SA))) {
                          if ((y > (centroid.y-SA)) && (y < (centroid.y+SA))) {
                             scare_mouse();
                             circlebres(screen,centroid.x,centroid.y,SA,makecol24(255,0,0));
                             unscare_mouse();
                             return currnode;
                          }
                       }
                    }
                    else {
                         if (currnode->Object == POLYGON) {
                            //textout(screen,font,"poly selected     ",1,1,makecol24(255,0,0));
                            scare_mouse();
                            drawpoly(currnode, makecol24(255,0,0));
                            unscare_mouse();
                            return currnode;
                         }
                         else {
                              if (currnode->Object == TEXT) {
                                 scare_mouse();
                                 textout(screen,font,currnode->obj_data.text.data,
                                         currnode->obj_data.text.p1.x,
                                         currnode->obj_data.text.p1.y,
                                         makecol24(255,0,0));
                                 unscare_mouse();
                                 return currnode;
                              }
                         }
                         
                    }


                } /* else */

                thelist->cursor = thelist->cursor->prev;
          } /* while */
    } /* while */


 return NULL;

} /* end */


void rotateobject(List thelist)
{

 Point centroid;
 double angle;
 double pie = 3.141592654;
 int x1;
 Point p[2]; //only lines
 int n;      //number of points in object (lines = 2)
 bool runonce = FALSE; //for centroid


 NodeLink thenode;
 thenode = thelist->cursor;

 if (thenode != NULL) {

     while (!(mouse_b & 2)) {
         if (mouse_b & 1) {
            x1 = mouse_x;

            while (mouse_b & 1) {
                  if (thenode->Object == LINE) {
                     n = 2;

                     //load nodes points into p[]
                     p[0].x = thenode->obj_data.line.p1.x;
                     p[0].y = thenode->obj_data.line.p1.y;
                     p[1].x = thenode->obj_data.line.p2.x;
                     p[1].y = thenode->obj_data.line.p2.y;

                     if (!(runonce)) {
                        centroid = calc_centroid(p,n);
                        runonce = TRUE;
                     }
                     
                     angle = (mouse_x - x1) / (XMAX / 100);
                     angle = ((pie/180) * angle); //angle in radians

                     rotate_n_poly(p,n,centroid,angle);

                     //unload from p into node
                     thenode->obj_data.line.p1.x = p[0].x;
                     thenode->obj_data.line.p1.y = p[0].y;
                     thenode->obj_data.line.p2.x = p[1].x;
                     thenode->obj_data.line.p2.y = p[1].y;

                     clearhack(screen, 0);
                     DrawList(thelist);

                  }
                  else {
                       if (thenode->Object == POLYGON) {

                          angle = (mouse_x - x1) / (XMAX / 100);
                          angle = ((pie/180) * angle); //angle in radians

                          if (!(runonce)) {
                             centroid.x = 1;
                             runonce = TRUE;
                          }

                          centroid = plistscalerotate(thenode->obj_data.polygon.theplist,
                                        centroid,
                                        angle,
                                        rotate_n_poly);

                          clearhack(screen, 0);
                          DrawList(thelist);
                       }
                  }/* else */

            }/* while m1 */
                //redraw all nodes
                //clear(screen);
                //DrawList(thelist);
         } /* if m1 */
     } /* while !m2 */

 }/* if node ! NULL */

}

void scaleobject(List thelist)
{
 Point centroid;
 double sfactor;
 Point p[2]; //only lines
 int n;      //number of points in object (lines = 2)
 bool runonce = FALSE; //for centroid


 NodeLink thenode;
 thenode = thelist->cursor;

 if (thenode != NULL) {

     while (!(mouse_b & 2)) {

         if (key[KEY_UP]) {
         
            sfactor = 1.01;

            if (thenode->Object == LINE) {
               n = 2;

                     //load nodes points into p[]
                     p[0].x = thenode->obj_data.line.p1.x;
                     p[0].y = thenode->obj_data.line.p1.y;
                     p[1].x = thenode->obj_data.line.p2.x;
                     p[1].y = thenode->obj_data.line.p2.y;

                     if (!(runonce)) {
                        centroid = calc_centroid(p,n);
                        runonce = TRUE;
                     }
                     
                     scale_n_poly(p,n,centroid,sfactor);

                     //unload from p into node
                     thenode->obj_data.line.p1.x = p[0].x;
                     thenode->obj_data.line.p1.y = p[0].y;
                     thenode->obj_data.line.p2.x = p[1].x;
                     thenode->obj_data.line.p2.y = p[1].y;

                  }
                  else {
                       if (thenode->Object == POLYGON) {

                          if (!(runonce)) {
                             centroid.x = 1;
                             runonce = TRUE;
                          }

                          centroid = plistscalerotate(thenode->obj_data.polygon.theplist,
                                        centroid,
                                        sfactor,
                                        scale_n_poly);

                       }
                  }/* else */
               sfactor = 0;
               clearhack(screen, 0);
               DrawList(thelist);
         }/* if key up */



         if (key[KEY_DOWN]) {
            sfactor = 0.99;

            if (thenode->Object == LINE) {
               n = 2;

                     //load nodes points into p[]
                     p[0].x = thenode->obj_data.line.p1.x;
                     p[0].y = thenode->obj_data.line.p1.y;
                     p[1].x = thenode->obj_data.line.p2.x;
                     p[1].y = thenode->obj_data.line.p2.y;

                     if (!(runonce)) {
                        centroid = calc_centroid(p,n);
                        runonce = TRUE;
                     }
                     
                     scale_n_poly(p,n,centroid,sfactor);

                     //unload from p into node
                     thenode->obj_data.line.p1.x = p[0].x;
                     thenode->obj_data.line.p1.y = p[0].y;
                     thenode->obj_data.line.p2.x = p[1].x;
                     thenode->obj_data.line.p2.y = p[1].y;

                  }
                  else {
                       if (thenode->Object == POLYGON) {

                          if (!(runonce)) {
                             centroid.x = 1;
                             runonce = TRUE;
                          }

                          centroid = plistscalerotate(thenode->obj_data.polygon.theplist,
                                        centroid,
                                        sfactor,
                                        scale_n_poly);

                       }
                  }/* else */
               sfactor = 0;
               clearhack(screen, 0);
               DrawList(thelist);
         }/* if key down */

     } /* while !m2 */

 }/* if node ! NULL */



}

void text(List thelist, int color)
{
 Text_Type textdata;
 int x,y;

 while (!(mouse_b & 2)) {

       if (mouse_b & 1) {
          x = mouse_x;
          y = mouse_y;

          scare_mouse();
          if (!(x < GUIWIDTH || x >= XMAX || y < 0 || y >= YMAX)) //clipcheck
             textout(screen,font,text_string,x,y,color);

             strncpy(textdata.data,text_string,42);
             textdata.p1.x = x;
             textdata.p1.y = y;
             textdata.color = color;
             textdata.width = 1;

             AddText(thelist,textdata);

          unscare_mouse();

          return;
       }
 }


}
/* sets thelists->cursor to the color from the d_box */
void setnodecolor(List thelist, int color)
{
 NodeLink thenode = thelist->cursor;

 if (thenode != NULL) {

    if (thenode->Object == LINE) {
        thenode->obj_data.line.color = color;
    }
    else {
         if (thenode->Object == CIRCLE) {
            thenode->obj_data.circle.color = color;
         }
         else {
              if (thenode->Object == POLYGON) {
                 thenode->obj_data.polygon.color = color;
              }
              else {
                   if (thenode->Object == TEXT) {
                      thenode->obj_data.text.color = color;
                   }
              }
         }
    }
    //redraw all nodes
    clearhack(screen,0);
    DrawList(thelist);
 }

}

/* makes a copy of thelist->cursor */
void copynode(List thelist)
{
 NodeLink thenode = thelist->cursor;
 Line_Type linedata;
 Poly_Type polydata;
 PList polylist,oldlist;
 Circle_Type circledata;
 Text_Type textdata;

 if (thenode != NULL) {

    if (thenode->Object == LINE) {
       linedata = thenode->obj_data.line;
       linedata.p1.x += 20;
       linedata.p1.y += 20;
       linedata.p2.x += 20;
       linedata.p2.y += 20;

       AddLine(thelist,linedata);

       linebres(linedata.p1.x,linedata.p1.y,linedata.p2.x,linedata.p2.y,makecol24(255,0,0));

    }
    else {
         if (thenode->Object == CIRCLE) {
            circledata = thenode->obj_data.circle;
            circledata.p1.x += 20;
            circledata.p1.y += 20;

            AddCircle(thelist,circledata);

            circlebres(screen,circledata.p1.x,circledata.p1.y,circledata.radius,makecol24(255,0,0));

         }
         else {
              if (thenode->Object == POLYGON) {
                 polydata = thenode->obj_data.polygon;
                 polylist = PCreateList();
                 oldlist =  thenode->obj_data.polygon.theplist;

                 oldlist->cursor = oldlist->first;
                 while (oldlist->cursor != NULL) {

                       PAddNode(polylist,oldlist->cursor->ppoint.x,oldlist->cursor->ppoint.y);
                       //alert("node added to copy",NULL,NULL,"ok",NULL,NULL,NULL);
                       oldlist->cursor = oldlist->cursor->next;
                 }

                 polylist->cursor = polylist->first;
                 while (polylist->cursor != NULL) {

                       polylist->cursor->ppoint.x += 20;
                       polylist->cursor->ppoint.y += 20;
                       //alert("point modified",NULL,NULL,"ok",NULL,NULL,NULL);
                       polylist->cursor = polylist->cursor->next;
                 }

                 polydata.theplist = polylist;

                 AddPolygon(thelist, polydata);

                 thenode = thelist->last;
                 drawpoly(thenode, makecol24(255,0,0));

              }
              else {
                   if (thenode->Object == TEXT) {
                      textdata = thenode->obj_data.text;
                      textdata.p1.x += 20;
                      textdata.p1.y += 20;

                      AddText(thelist, textdata);

                      textout(screen,font,textdata.data,textdata.p1.x,textdata.p1.y,makecol24(255,0,0));
                   }
              }
         }
    }

 }

}


void Save(List thelist)
{
 char path[255] = "c:/";
 int ret;
 FILE *fp;
 BITMAP *bmp;
 PALETTE pal;

 ret = file_select("Save Bitmap As...",path,"BMP");
 //alert("Sorry this feature is","broken at the moment :)",NULL,"Thats Ok","ACK!",NULL,NULL);

 if (ret) {
    /*if ((fp = fopen(path,"wb"))==NULL)
       alert("cant open file:",path,NULL,"OK","ACK!",NULL,NULL);
       return;
    ret = fwrite(&thelist,sizeof(List),1,fp);
    if (ret == 1) {
       alert("nothing writen to:",path,NULL,"OK","ACK!",NULL,NULL);
    }
    fclose(fp);*/
    //wang_dialog[T_B].flags = D_HIDDEN;
    //SEND_MESSAGE(&wang_dialog[T_B], MSG_DRAW,0);

    clearhack(screen,0);
    DrawList(thelist);

    get_palette(pal);
    bmp = create_sub_bitmap(screen,GUIWIDTH,0,SCREEN_W-GUIWIDTH, SCREEN_H);
    save_bmp(path,bmp,pal);
    destroy_bitmap(bmp);

    //wang_dialog[T_B].flags = D_EXIT;
    //SEND_MESSAGE(&wang_dialog[T_B], MSG_DRAW,0);
 }

}

void SetDialogColors(void) {
	int black, white;
	black = makecol24(0,0,0);
	white = makecol24(255,255,255);

        wang_dialog[0].fg=white;
        wang_dialog[0].bg=black;
        wang_dialog[1].fg=white;
	wang_dialog[1].bg=black;
	wang_dialog[2].fg=white;
	wang_dialog[2].bg=black;
	wang_dialog[3].fg=white;
	wang_dialog[3].bg=black;
	wang_dialog[4].fg=white;
	wang_dialog[4].bg=black;
	wang_dialog[5].fg=white;
	wang_dialog[5].bg=black;
	wang_dialog[6].fg=white;
	wang_dialog[6].bg=black;
	wang_dialog[7].fg=white;
	wang_dialog[7].bg=black;
	wang_dialog[8].fg=white;
	wang_dialog[8].bg=black;
	wang_dialog[9].fg=white;
	wang_dialog[9].bg=black;
	wang_dialog[10].fg=white;
	wang_dialog[10].bg=black;
	wang_dialog[11].fg=white;
	wang_dialog[11].bg=black;
	wang_dialog[12].fg=white;
	wang_dialog[12].bg=black;
        wang_dialog[13].fg=white;
	wang_dialog[13].bg=white;
        wang_dialog[14].fg = makecol24(255,0,0);
        wang_dialog[15].fg = makecol24(0,255,0);
        wang_dialog[16].fg = makecol24(0,0,255);
        wang_dialog[17].fg = black;
        wang_dialog[17].bg = white;
        wang_dialog[18].fg = white;
        wang_dialog[18].bg = black;
        
}

void redrawmenu()
{
 int i;

 for (i=0;i<=17;i++)
     SEND_MESSAGE(&wang_dialog[i], MSG_DRAW, 0);

}

int update_color(void *dp3, int d2)
{

   wang_dialog[BOX_COLOR].bg = makecol24(wang_dialog[S_RED].d2, wang_dialog[S_GREEN].d2, wang_dialog[S_BLUE].d2);

   SEND_MESSAGE(&wang_dialog[BOX_COLOR], MSG_DRAW,0);

   return D_O_K;
}

int line_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {
    rubberline(wang->dp2, wang_dialog[BOX_COLOR].bg);

    SEND_MESSAGE(&wang_dialog[B_L], MSG_DRAW,0);

    return D_O_K;
 }

 return D_O_K;

}

int polyline_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {
    rubberpolygon(wang->dp2, wang_dialog[BOX_COLOR].bg);

    SEND_MESSAGE(&wang_dialog[B_P], MSG_DRAW,0);

    return D_O_K;
 }

 return D_O_K;

}

int circle_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {
    rubbercircle(wang->dp2, wang_dialog[BOX_COLOR].bg);

    SEND_MESSAGE(&wang_dialog[B_C], MSG_DRAW,0);

    return D_O_K;
 }

 return D_O_K;

}

int rect_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {
    rubberrectangle(wang->dp2, wang_dialog[BOX_COLOR].bg);

    SEND_MESSAGE(&wang_dialog[B_R], MSG_DRAW,0);

    return D_O_K;
 }

 return D_O_K;

}

int rotate_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {
    rotateobject(wang->dp2);

    SEND_MESSAGE(&wang_dialog[B_Ro], MSG_DRAW,0);

    return D_O_K;
 }

 return D_O_K;

}

int scale_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {
    scaleobject(wang->dp2);

    SEND_MESSAGE(&wang_dialog[B_Sc], MSG_DRAW,0);

    return D_O_K;
 }

 return D_O_K;

}

int selectmode_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {
    SelectMode(wang->dp2);

    SEND_MESSAGE(&wang_dialog[B_S], MSG_DRAW,0);

    return D_REDRAW;
 }

 return D_O_K;

}

int text_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {

    text(wang->dp2, wang_dialog[BOX_COLOR].bg);

    return D_REDRAW;

 }
 
 return D_O_K;

}

int copynode_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {

    copynode(wang->dp2);

    scare_mouse();
    SEND_MESSAGE(&wang_dialog[B_Cop], MSG_DRAW,0);
    unscare_mouse();
    
    return D_O_K;

 }
 
 return D_O_K;

}

int colchange_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {

    setnodecolor(wang->dp2, wang_dialog[BOX_COLOR].bg);

    scare_mouse();
    SEND_MESSAGE(&wang_dialog[B_Col], MSG_DRAW,0);
    unscare_mouse();
    
    return D_O_K;

 }
 
 return D_O_K;

}



int save_proc(int msg, DIALOG *wang, int c)
{
 int ret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {
    Save(wang->dp2);

    SEND_MESSAGE(&wang_dialog[B_Sa], MSG_DRAW,0);

    return D_O_K;
 }

 return D_O_K;

}

int exit_proc(int msg, DIALOG *wang, int c)
{
 int ret,aret;

 ret = d_button_proc(msg, wang, c);

 if (ret == D_CLOSE) {
    aret = alert("Do you want to save before exiting?",NULL,NULL,"Yes","No Thanks",NULL,NULL);
    if (aret == 1) {
       Save(wang->dp2);
    }
    aret = alert("Are you sure you want to quit?",NULL,NULL,"Ok","Cancel",NULL,NULL);
    if (aret == 1)
       return D_EXIT;

    SEND_MESSAGE(&wang_dialog[B_Ex], MSG_DRAW,0);
    return D_O_K;
 }

 return D_O_K;

}

/* original code from the allegro source code
   changed for faster clearing of the work area
 */
void clearhack(BITMAP *dst, int color)
{
   int x, y;
   int w = dst->cr - dst->cl;

   bmp_select(dst);

   for (y = dst->ct; y < dst->cb; y++) {
      PIXEL_PTR d = (OFFSET_PIXEL_PTR(bmp_write_line(dst, y), dst->cl)) + 192;

      for (x = w - 1; x >= 0; INC_PIXEL_PTR(d), x--) {
	 PUT_PIXEL(d, color);
      }
   }

   bmp_unwrite_line(dst);
}
