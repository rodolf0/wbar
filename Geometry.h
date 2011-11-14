#ifndef _GEOMETRY_
#define _GEOMETRY_

class GeometryObject {
  public:
    virtual ~GeometryObject() {};
};

class Point : public GeometryObject {
  public:
    Point(int x, int y);
    int x, y;
};

typedef Point Size;

class Rect : public GeometryObject {
  public:
    Rect(int x, int y, int w, int h);
    int x, y;
    int width, height;
};

class Border : public GeometryObject {
  public:
    Border(int l, int r, int t, int b);
    int left, right, top, bottom;
};

#endif /* _GEOMETRY_ */

/* vim: set sw=2 sts=2 : */
