// globals.h

#ifndef GLOBALSHEADER
#define GLOBALSHEADER

#define XMAX 800
#define YMAX 600
#define COLORDEPTH 24
#define GUIWIDTH 64

typedef struct {

    int x;
    int y;
    int color;
} pixel;

typedef struct {
        int x,y;
} Point;

pixel savedpixels[XMAX * YMAX];	/* array to store pixel info */
int pos_sp;                     /* position of savedpixels array */

typedef int bool;
#undef TRUE
#define TRUE 1
#undef FALSE
#define FALSE 0

//dialog defines
#define B_S 1
#define B_MS 2
#define B_L 3
#define B_C 4
#define B_P 5
#define B_R 6
#define B_Sc 7
#define B_Ro 8
#define B_T 9
#define B_Col    10
#define B_Sa      11
#define B_Ex      12
#define BOX_COLOR 13
#define S_RED     14
#define S_GREEN   15
#define S_BLUE    16
#define T_B       17
#define B_Cop     18

#endif
