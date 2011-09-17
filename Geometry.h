#ifndef _GEOMETRY_
#define _GEOMETRY_

class Layout {
  public:
    virtual ~Layout() = 0;
};

class Layout2D : public Layout {
  public:
    Layout2D();
    Layout2D(float x, float y, float w, float h, float a = 0.0);
};

class Layout3D : public Layout {
  public:
    Layout3D();
    Layout3D(float x, float y, float z, float w, float h,
             float ax, float ay, float az);
};


class RectLayout : public Layout {
  public:
    int x, y;
    int width, height;

    RectLayout();
    RectLayout(int x, int y, int w, int h);
};

class Point {
  public:
    int x, y;

    Point(int x, int y);
};

typedef Point Size;


//class Rect2D {
  //public:
    //float cx, cy;
    //float w, h;
    //float angle;
//};


//class Rect3D {
  //public:
    //float cx, cy, cz;
    //float w, h;
    //float ax, ay, az;
//};

#endif /* _GEOMETRY_ */

/* vim: set sw=2 sts=2 : */
