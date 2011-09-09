#include "Dock.h"

Dock::Dock(CanvasEngine &ce, LayoutStrategy &ls) :
    Xwindow(Rect(0, 0, 1, 1),
            PointerMotionMask | ExposureMask | ButtonPressMask |
            ButtonReleaseMask | LeaveWindowMask | EnterWindowMask),
    layout(ls), canvas_engine(ce) {
  // setup
}

void Dock::addWidget(const Widget &w) {
  widgets.push_back(w);
  layout.resize(widgets.size());
}

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

}

/* vim: set sw=2 sts=2 : */
