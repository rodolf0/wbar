#ifndef _DOCK_
#define _DOCK_

#include <vector>
#include "LayoutStrategy.h"
#include "Xwindow.h"
#include "CanvasEngine.h"
#include "Widget.h"

class Dock : public Xwindow {
  private:
    std::vector<Widget> widgets;
    LayoutStrategy &layout;
    CanvasEngine &canvas_engine;

  public:
    Dock(CanvasEngine &ce, LayoutStrategy &ls);
    ~Dock();

    void addWidget(const Widget &w);
    void removeWidget(const Widget &w);
    void render();

    void onExposure(const XExposeEvent &e);
    void onMouseMove(const XMotionEvent &e);
    void onMouseDown(const XButtonEvent &e);
    void onMouseUp(const XButtonEvent &e);
    void onMouseEnter(const XCrossingEvent &e);
    void onMouseLeave(const XCrossingEvent &e);
};

#endif /* _DOCK_ */

/* vim: set sw=2 sts=2 : */
