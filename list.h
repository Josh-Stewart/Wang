// linked list header

#ifndef LISTHEADER
#define LISTHEADER

//for Point + bool typedefs
#include "globals.h"


typedef enum {LINE,CIRCLE,POLYGON,TEXT} ObjectType;

//polygon linked list of points ds
typedef struct PListObj  *PList;    //points to the poly list
typedef struct PNode     *PNodeLink;//points to a poly node

struct PNode{

        PNodeLink prev,next;

        Point ppoint;
};

struct PListObj{

       PNodeLink cursor;
       PNodeLink first,last;

       size_t length;
       size_t cur_pos;
};
//end polygon ds


typedef struct {

        Point p1,p2;
        int color;
        int width;

}Line_Type;

typedef struct {

        char data[42]; // sadly this is static, its pretty much the only thing that is (hey im out of time) :(
        Point p1;      // starting point of text
        //FONT *font;  // havent quite figured this out yet
        int color;
        int width;

}Text_Type;

typedef struct {

        Point p1;
        int radius;
        int color, fill_color;
        bool filled;
        int width;

}Circle_Type;

typedef struct {

        PList theplist;
        int color, fill_color;
        bool filled;
        int width;

}Poly_Type;


typedef struct ListObj    *List;     //points to the list
typedef struct Node   *NodeLink; //points to a node

struct Node{

        NodeLink prev,next;   /* list pointers */
        
        ObjectType Object;    /* what objecttype is in this node */
        union {
              Line_Type line;
              Circle_Type circle;
              Poly_Type polygon;
              Text_Type text;
              
        }obj_data;
};

struct ListObj {          /* double linked list of nodes */

        NodeLink cursor;
        NodeLink first,last;

        size_t length;
        size_t cur_pos;  //size_t = long unsigned int

};


// Function Prototypes

void AddLine(List thelist, Line_Type linedata);
void AddCircle(List thelist, Circle_Type circledata);
void AddText(List thelist, Text_Type textdata);
void AddPolygon(List thelist, Poly_Type polydata);
List CreateList();
bool IsEmptyList(List thelist);
void MakeEmptyList(List thelist);
void DisposeList(List thelist);
void DeleteNode(List thelist);

//poly list functions (so much for reusability ;)
PList PCreateList();
void PAddNode(PList polylist,int x,int y); //too much hassel passin in a Point :)_
bool IsEmptyPList(PList polylist);
void MakeEmptyPList(PList polylist);
void DisposePList(PList polylist);

#endif
