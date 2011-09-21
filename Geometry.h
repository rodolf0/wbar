#ifndef _GEOMETRY_
#define _GEOMETRY_

class Layout {
  public:
    virtual ~Layout() = 0;
};


class Layout2D : public Layout {
  public:
    Layout2D(float x, float y, float w, float h, float a = 0.0);
};

class Layout3D : public Layout {
  public:
    Layout3D(float x, float y, float z, float w, float h,
             float ax, float ay, float az);
};


class RectLayout : public Layout {
  public:
    int x, y;
    int width, height;

    RectLayout(int x, int y, int w, int h);
};


class Point {
  public:
    int x, y;

    Point(int x, int y);
};

typedef Point Size;


#endif /* _GEOMETRY_ */

/* vim: set sw=2 sts=2 : */
