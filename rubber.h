// rubberline.h

#ifndef RUBBERLINEHEADER
#define RUBBERLINEHEADER


//these are internal to rubber.c and arent used outside

//void save_line(BITMAP * bmp, int x, int y, int d);
//void restore_line(BITMAP * bmp);

//these arent :)

void rubberline(List thelist, int color);
void rubbercircle(List thelist, int color);
void rubberpolygon(List thelist, int color);
void rubberrectangle(List thelist, int color);
#endif
