#include "Dock.h"

Dock::Dock(LayoutStrategy &ls) : layout(ls) { }
//Dock::Dock(Image &bg, LayoutStrategy &ls) :
    //dock_background(bg), layout(ls) { }

Dock::~Dock() {}

void Dock::onExposure(const XExposeEvent &e) {
  layout.focus(Point(e.x, e.y));
  CanvasEngine::get().render();
}

void Dock::onMouseEnter(const XCrossingEvent &e) {
  layout.focus(Point(e.x, e.y));
  CanvasEngine::get().render();
}


void Dock::onMouseLeave(const XCrossingEvent &e) {
  layout.unfocus();
  CanvasEngine::get().render();
}


void Dock::onMouseMove(const XMotionEvent &e) {
  const Point p(e.x, e.y);
  if (layout.atHoverZone(p)) {
    layout.focus(p);
  } else {
    layout.unfocus();
  }
  CanvasEngine::get().render();
}


void Dock::onMouseDown(const XButtonEvent &e) {
  int idx = layout.widgetAt(Point(e.x, e.y));
}


void Dock::onMouseUp(const XButtonEvent &e) {
  int idx = layout.widgetAt(Point(e.x, e.y));
}

/* vim: set sw=2 sts=2 : */
