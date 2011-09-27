#ifndef _GEOMETRY_
#define _GEOMETRY_

class Point {
  public:
    int x, y;

    Point(int x, int y);
};

typedef Point Size;

class Rect {
  public:
    int x, y;
    int width, height;

    Rect(int x, int y, int w, int h);
};

#endif /* _GEOMETRY_ */

/* vim: set sw=2 sts=2 : */
