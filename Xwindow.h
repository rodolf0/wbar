#ifndef _XWINDOW_
#define _XWINDOW_

#include <X11/Xlib.h>
#include "Geometry.h"

class Xwindow {
  public:
    Xwindow(const Point &position, const Size &size);
    virtual ~Xwindow();

    virtual void setGeometry(int x, int y, int w, int h);
    virtual void map() const;

    Window getWindow() const;
    static Display * getDisplay();

    size_t width() const;
    size_t height() const;

  protected:
    static Display *display;
    Window window;
    Point position;
    Size dimensions;
};


class XEventHandler {
  public:
    virtual ~XEventHandler() = 0;

    virtual void onExposure(const XExposeEvent &e) = 0;
    virtual void onMouseMove(const XMotionEvent &e) = 0;
    virtual void onMouseDown(const XButtonEvent &e) = 0;
    virtual void onMouseUp(const XButtonEvent &e) = 0;
    virtual void onMouseEnter(const XCrossingEvent &e) = 0;
    virtual void onMouseLeave(const XCrossingEvent &e) = 0;

    static unsigned long eventMask();
    virtual void eventLoop(Xwindow &w);
};

#endif /* _XWINDOW_ */

/* vim: set sw=2 sts=2 : */
