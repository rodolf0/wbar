#include "Geometry.h"


Layout::~Layout() {}


RectLayout::RectLayout(int x, int y, int w, int h) :
    x(x), y(y), width(w), height(h) { }


Point::Point(int x, int y) :
    x(x), y(y) { }

/* vim: set sw=2 sts=2 : */
