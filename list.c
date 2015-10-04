/* active linked list implementation

   These are the functions which operate on the data structure
   defined in list.h

   -JS
 */
#include <stdlib.h>
#include <stdio.h>
#include "allegro.h"
#include "list.h"

void AddLine(List thelist, Line_Type linedata)
{
 NodeLink newnode;
 NodeLink oldend;

 newnode = malloc(sizeof(struct Node));

 //fill up new node
 newnode->next = NULL;
 newnode->Object = LINE;
 newnode->obj_data.line = linedata;

 if (thelist->length == 0){

    thelist->last = newnode;
    thelist->first= newnode;
    newnode->prev = NULL;
    newnode->next = NULL;
    
 }
 else {
      newnode->prev = thelist->last;

      oldend = thelist->last;
      oldend->next = newnode;
      thelist->last = newnode;
 }

 thelist->length++;


}

void AddText(List thelist, Text_Type textdata)
{
 NodeLink newnode;
 NodeLink oldend;

 newnode = malloc(sizeof(struct Node));

 //fill up new node
 newnode->next = NULL;
 newnode->Object = TEXT;
 newnode->obj_data.text = textdata;

 if (thelist->length == 0){

    thelist->last = newnode;
    thelist->first= newnode;
    newnode->prev = NULL;
    newnode->next = NULL;
    
 }
 else {
      newnode->prev = thelist->last;

      oldend = thelist->last;
      oldend->next = newnode;
      thelist->last = newnode;
 }

 thelist->length++;


}


void AddPolygon(List thelist, Poly_Type polydata)
{
 NodeLink newnode;
 NodeLink oldend;

 newnode = malloc(sizeof(struct Node));

 //fill new pnode
 newnode->next = NULL;
 newnode->Object = POLYGON;
 newnode->obj_data.polygon = polydata;

 if (thelist->length == 0){
    thelist->last = newnode;
    thelist->first= newnode;
    newnode->prev = NULL;
    newnode->next = NULL;
 }
 else {
      newnode->prev = thelist->last;
      oldend = thelist->last;
      oldend->next = newnode;
      thelist->last = newnode;
 }

 thelist->length++;

}


void AddCircle(List thelist, Circle_Type circledata)
{
 NodeLink newnode;
 NodeLink oldend;

 newnode = malloc(sizeof(struct Node));

 //fill up new node
 newnode->next = NULL;
 newnode->Object = CIRCLE;
 newnode->obj_data.circle = circledata;

 if (thelist->length == 0){

    thelist->last = newnode;
    thelist->first= newnode;
    newnode->prev = NULL;
    newnode->next = NULL;
    
 }
 else {
      newnode->prev = thelist->last;

      oldend = thelist->last;
      oldend->next = newnode;
      thelist->last = newnode;
 }

 thelist->length++;


}


List CreateList()
{
 List newlist;

 newlist = malloc(sizeof(struct ListObj));

 newlist->cursor = NULL;
 newlist->first = NULL;
 newlist->last = NULL;
 newlist->length = 0;
 newlist->cur_pos= 0;

 return newlist;
}

PList PCreateList()
{
 PList newlist;

 newlist = malloc(sizeof(struct PListObj));

 newlist->cursor = NULL;
 newlist->first = NULL;
 newlist->last = NULL;
 newlist->length = 0;
 newlist->cur_pos = 0;

 return newlist;
}
 
void PAddNode(PList polylist,int x, int y)
{
 PNodeLink newnode;
 PNodeLink oldend;
 Point newpoint;

 newpoint.x = x;
 newpoint.y = y;

 newnode = malloc(sizeof(struct PNode));

 //fill up new node
 newnode->next = NULL;
 newnode->ppoint = newpoint;

 if (polylist->length == 0){

    polylist->last = newnode;
    polylist->first= newnode;
    newnode->prev = NULL;
    newnode->next = NULL;
    
 }
 else {
      newnode->prev = polylist->last;

      oldend = polylist->last;
      oldend->next = newnode;
      polylist->last = newnode;
 }

 polylist->length++;

}

bool IsEmptyPList(PList polylist)
{
 if (polylist->length ==0)
    return TRUE;
 else
     return FALSE;
}


bool IsEmptyList(List thelist)
{
 if (thelist->length == 0)
    return TRUE;
 else
     return FALSE;
}

void MakeEmptyList(List thelist)
{
 NodeLink curr;

 while (!(IsEmptyList(thelist))) {

       curr = thelist->last;
       thelist->last = curr->prev;

       free(curr);

       thelist->length--;
 }
}

void MakeEmptyPList(PList polylist)
{
 PNodeLink curr;

 while (!(IsEmptyPList(polylist))) {

       curr = polylist->last;
       polylist->last = curr->prev;

       free(curr);

       polylist->length--;
 }
}

void DisposeList(List thelist)
{
 MakeEmptyList(thelist);
 free(thelist);
}

void DisposePList(PList polylist)
{
 MakeEmptyPList(polylist);
 free(polylist);
}


void DeleteNode(List thelist)
{
 //delete the list->cursor node
 NodeLink curr, a, b;

 curr = thelist->cursor;
 a = curr->prev;
 b = curr->next;

 if (a != NULL && b != NULL) {
    a->next = b;
    b->prev = a;
 }
 else {
      if (a == NULL && b == NULL) {
         thelist->first = NULL;
         thelist->last = NULL;
      }
      else {
           if (b == NULL) {
              a->next = NULL;
              thelist->last = a;
           }
           else {
                if (a == NULL) {
                   b->prev = NULL; // Jon fix .. was b->prev == NULL;
                   thelist->first = b;
                }
           }
      }
 }

 thelist->length--;

 thelist->cursor = NULL;

 free(curr);

}

