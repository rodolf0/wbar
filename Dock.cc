#include "Dock.h"

Dock::Dock(Xwindow &w, Image &bg, LayoutStrategy &ls) :
    XEventHandler(w), dock_background(bg), layout(ls) { }


void Dock::onMouseEnter(const XCrossingEvent &e) {
  layout.focus(Point(e.x, e.y));
  render();
}


void Dock::onMouseLeave(const XCrossingEvent &e) {
  layout.unfocus();
  render();
}


void Dock::onMouseMove(const XMotionEvent &e) {
  const Point p(e.x, e.y);
  if (layout.atHoverZone(p)) {
    layout.focus(p);
    render();
  } else {
    layout.unfocus();
    render();
  }
}


void Dock::onMouseDown(const XButtonEvent &e) {
}


void Dock::onMouseUp(const XButtonEvent &e) {
}


void Dock::render() {
  //CanvasEngine &c = CanvasEngine::get();
}

/* vim: set sw=2 sts=2 : */
