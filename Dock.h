#ifndef _DOCK_
#define _DOCK_

#include "LayoutStrategy.h"
#include "CanvasEngine.h"
#include "Xwindow.h"

class Dock : public XEventHandler {
  private:
    //Image &dock_background;
    LayoutStrategy &layout;
    void render();

  public:
    //Dock(Image &bg, LayoutStrategy &ls); // std::vector<Widget *> &widgets
    Dock(LayoutStrategy &ls); // std::vector<Widget *> &widgets
    ~Dock();

    void onExposure(const XExposeEvent &e);
    void onMouseMove(const XMotionEvent &e);
    void onMouseDown(const XButtonEvent &e);
    void onMouseUp(const XButtonEvent &e);
    void onMouseEnter(const XCrossingEvent &e);
    void onMouseLeave(const XCrossingEvent &e);
};

#endif /* _DOCK_ */

/* vim: set sw=2 sts=2 : */
