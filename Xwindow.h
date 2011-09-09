#ifndef _XWINDOW_
#define _XWINDOW_

#include <X11/Xlib.h>
#include "Geometry.h"

class Xwindow {
  private:
    Display *d;
    Window w;
    size_t pos_x, pos_y;
    size_t  width, height;
    unsigned long ev_mask;

  public:
    Xwindow(const Rect &r, unsigned long evmask);
    virtual ~Xwindow() = 0;

    virtual void onExposure(const XExposeEvent &e);
    virtual void onMouseMove(const XMotionEvent &e);
    virtual void onMouseDown(const XButtonEvent &e);
    virtual void onMouseUp(const XButtonEvent &e);
    virtual void onMouseEnter(const XCrossingEvent &e);
    virtual void onMouseLeave(const XCrossingEvent &e);

    virtual void eventLoop();
};

#endif /* _XWINDOW_ */

/* vim: set sw=2 sts=2 : */
