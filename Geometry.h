#ifndef _GEOMETRY_
#define _GEOMETRY_

class Point {
  public:
    Point(int x, int y);
    int x, y;
};

typedef Point Size;

class Rect {
  public:
    Rect(int x, int y, int w, int h);
    int x, y;
    int width, height;
};

#endif /* _GEOMETRY_ */

/* vim: set sw=2 sts=2 : */
