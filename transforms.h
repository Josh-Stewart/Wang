// transforms.h

#ifndef TRANSFORMSHEADER
#define TRANSFORMSHEADER

//prototypes
void rotate(double pnt[], double radian);
void scale(double pnt[], double sfactor);
void move(double pnt[], Point xy_offset);
//void drawpoly(Point p[], int n,int color);
//void make_n_poly(Point p[], int n);
void rotate_n_poly(Point p[], int n, Point centroid, double angle);
void scale_n_poly(Point p[], int n, Point centroid, double sfactor);
void move_n_poly(Point p[], int n, Point centroid, Point xy_offset);
Point calc_centroid(Point p[], int n);

void plistmove(PList polylist, Point new_xy);
Point plistscalerotate(PList polylist, Point centroid, double scalerotate, void (*proc)(Point *,int ,Point ,double ));
#endif
