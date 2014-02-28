#ifndef _XWINDOW_
#define _XWINDOW_

#include <X11/Xlib.h>
#include "Geometry.h"

enum winlayer {
  wlayer_below = 0,
  wlayer_above = 6
};

enum wintype {
  wtype_normal = 0,
  wtype_dock,
  wtype_desktop
};

class Xwindow {
  public:
    Xwindow(const Size &size);
    virtual ~Xwindow();

    virtual void resize(const Size &size) const;
    virtual void move(const Point &pos) const;
    virtual void map() const;
    virtual void hide() const;

    Window getWindow() const;
    static Display * getDisplay();
    int getDepth() const;
    Colormap getColormap() const;
    Visual * getVisual() const;
    int getScreen() const;

    static Size screenSize();

    size_t width() const;
    size_t height() const;

    void setType(wintype wtype);
    void setSticky();
    void setSkipPager();
    void setSkipTaskbar();
    void setLayer(winlayer layer);
    void setOverrideRedirect(bool on);
    void decorationsOff();

  protected:
    // disallow copy constructor and assignment (pointer members)
    Xwindow(const Xwindow &);
    Xwindow & operator=(const Xwindow &);

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

    virtual void onExposure(const XExposeEvent &) {}
    virtual void onMouseMove(const XMotionEvent &) {}
    virtual void onMouseDown(const XButtonEvent &) {}
    virtual void onMouseUp(const XButtonEvent &) {}
    virtual void onMouseEnter(const XCrossingEvent &) {}
    virtual void onMouseLeave(const XCrossingEvent &) {}

    static unsigned long eventMask();
    virtual void eventLoop(Xwindow &w);
};

#endif /* _XWINDOW_ */

/* vim: set sw=2 sts=2 : */
