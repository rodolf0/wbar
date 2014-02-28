#include "Geometry.h"

Point::Point(int x_, int y_) :
    x(x_), y(y_) { }

Rect::Rect(int x_, int y_, int w_, int h_) :
    x(x_), y(y_), width(w_), height(h_) { }

Border::Border(int l_, int r_, int t_, int b_) :
    left(l_), right(r_), top(t_), bottom(b_) { }

/* vim: set sw=2 sts=2 : */
