#include "CanvasLayouts.h"

Layout::~Layout() {}

RectLayout::RectLayout(const Rect &r) : layout(r) {}

void RectLayout::transform(Evas_Object *img) const {
  evas_object_resize(img, layout.width, layout.height);
  evas_object_move(img, layout.x, layout.y);
}

/* vim: set sw=2 sts=2 : */
