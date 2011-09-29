#ifndef _XWINDOW_
#define _XWINDOW_

#include <X11/Xlib.h>
#include "Geometry.h"

class Xwindow {
  public:
    Xwindow(const Size &size);
    virtual ~Xwindow();

    virtual void resize(const Size &size) const;
    virtual void move(const Point &pos) const;
    virtual void map() const;
    virtual void hide() const;
    //virtual void hints();

    Window getWindow() const;
    static Display * getDisplay();
    int getDepth() const;
    Colormap getColormap() const;
    Visual * getVisual() const;
    int getScreen() const;


    size_t width() const;
    size_t height() const;


  protected:
    Window window;
    Colormap colormap;
    Visual *visual;
    int depth;

    void registerDelete() const;

    static Display *display;
    static size_t win_count;
    static Visual * findARGB32Visual(int screen);
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
