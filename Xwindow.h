#ifndef _XWINDOW_
#define _XWINDOW_

#include <X11/Xlib.h>
#include "Geometry.h"

class Xwindow {
  public:
    Xwindow();
    virtual ~Xwindow();
    virtual void setGeometry();
    Window getWindow() const;

    static Display * getDisplay();

  protected:
    static Display *display;
    Window window;
    RectLayout layout;
};


class XEventHandler {
  public:
    XEventHandler(Xwindow &w);
    virtual ~XEventHandler() = 0;

    virtual void onExposure(const XExposeEvent &e);
    virtual void onMouseMove(const XMotionEvent &e);
    virtual void onMouseDown(const XButtonEvent &e);
    virtual void onMouseUp(const XButtonEvent &e);
    virtual void onMouseEnter(const XCrossingEvent &e);
    virtual void onMouseLeave(const XCrossingEvent &e);

    static unsigned long eventMask();
    virtual void eventLoop();

  protected:
    Xwindow &window;
};

#endif /* _XWINDOW_ */

/* vim: set sw=2 sts=2 : */
