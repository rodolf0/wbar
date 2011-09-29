#include <X11/extensions/Xrender.h>
#include "Xwindow.h"

#include <iostream>

Display *Xwindow::display = NULL;
size_t Xwindow::win_count = 0;

Visual * Xwindow::findARGB32Visual(int screen) {
  Visual *v = NULL;
  int event_base, error_base;

  if (XRenderQueryExtension(display, &event_base, &error_base)) {
    XVisualInfo vi_template, *xvi;
    int num_vi;

    vi_template.screen = screen;
    vi_template.depth = 32;
    vi_template.c_class = TrueColor;

    xvi = XGetVisualInfo(display,
                         VisualScreenMask | VisualDepthMask | VisualClassMask,
                         &vi_template, &num_vi);

    for (int i = 0; i < num_vi; i++) {
      XRenderPictFormat *fmt = XRenderFindVisualFormat(display, xvi[i].visual);
      if (fmt->type == PictTypeDirect && fmt->direct.alphaMask) {
        v = xvi[i].visual;
        break;
      }
    }
    XFree(xvi);
  }
  return v;
}


// get event on window deletion
void Xwindow::registerDelete() const {
  Atom delwin = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, window, &delwin, 1);
}


Xwindow::Xwindow(const Size &size) {
  if (win_count == 0)
    if (!(display = XOpenDisplay(NULL)))
      throw "ERROR: XOpenDisplay failed.";

  int screen = DefaultScreen(display);
  Window root = RootWindow(display, screen);

  XSetWindowAttributes wa = {
    .backing_store = NotUseful,
    .border_pixel = 0,
    .background_pixmap = None,
    .bit_gravity = ForgetGravity,
    //.override_redirect = 1,
  };

  if ((visual = findARGB32Visual(screen))) {
    depth = 32;
    wa.colormap = XCreateColormap(display, root, visual, AllocNone);
  } else {
    throw "ERRRO: No ARGB32 visual.";
    //visual = DefaultVisual(display, screen);
    //depth = DefaultDepth(display, screen);
    //wa.colormap = DefaultColormap(display, screen);
    // Signal copying background... no real transparency
    // http://svn.exactcode.de/macosd/trunk/bin/PBBEvas.cc
  }

  if (!(window = XCreateWindow(display, root, 0, 0,
                               size.x, size.y, 0, depth, InputOutput, visual,
                               //CWOverrideRedirect |
                               CWBackingStore | CWColormap | CWBackPixmap |
                               CWBorderPixel | CWBitGravity, &wa))) {
    XFreeColormap(display, colormap);
    XCloseDisplay(display);
    throw "ERRRO: XCreateWindow failed.";
  }

  registerDelete();
  win_count++;
}


Xwindow::~Xwindow() {
  win_count--;
  XDestroyWindow(display, window);
  XFreeColormap(display, colormap);
  if (win_count == 0)
    XCloseDisplay(display);
}


Display * Xwindow::getDisplay() { return display; }
Window Xwindow::getWindow() const { return window; }
int Xwindow::getDepth() const { return depth; }
Colormap Xwindow::getColormap() const { return colormap; }
Visual * Xwindow::getVisual() const { return visual; }
int Xwindow::getScreen() const { return DefaultScreen(display); }


void Xwindow::move(const Point &pos) const {
  XMoveWindow(display, window, pos.x, pos.y);
}
void Xwindow::resize(const Size &size) const {
  XResizeWindow(display, window, size.x, size.y);
}

void Xwindow::map() const { XMapWindow(display, window); }
void Xwindow::hide() const { XUnmapWindow(display, window); }


size_t Xwindow::width() const {
  int x, y;
  unsigned int w, h, bw, depth;
  Window root;
  XGetGeometry(display, window, &root,
               &x, &y, &w, &h, &bw, &depth);
  return w;
}

size_t Xwindow::height() const {
  int x, y;
  unsigned int w, h, bw, depth;
  Window root;
  XGetGeometry(display, window, &root,
               &x, &y, &w, &h, &bw, &depth);
  return h;
}



XEventHandler::~XEventHandler() {}

void XEventHandler::eventLoop(Xwindow &w) {
  bool process_events = true;

  XSelectInput(w.getDisplay(), w.getWindow(), eventMask());

  while (process_events) {
    XEvent ev;
    XWindowEvent(w.getDisplay(), w.getWindow(), eventMask(), &ev);

    switch (ev.type) {
      case EnterNotify:
        if (ev.xcrossing.mode != NotifyGrab &&
            !(ev.xcrossing.state & Button1Mask))
          onMouseEnter(ev.xcrossing);
        break;
      case LeaveNotify:
        if (ev.xcrossing.mode != NotifyGrab &&
            !(ev.xcrossing.state & Button1Mask))
          onMouseLeave(ev.xcrossing);
        break;
      case MotionNotify:
        onMouseMove(ev.xmotion);
        break;
      case ButtonPress:
        onMouseDown(ev.xbutton);
        break;
      case ButtonRelease:
        onMouseUp(ev.xbutton);
        break;
      case Expose:
        onExposure(ev.xexpose);
        break;
      default:
        break;
    }
  }
}


unsigned long XEventHandler::eventMask() {
  return PointerMotionMask | ExposureMask | ButtonPressMask |
         ButtonReleaseMask | LeaveWindowMask | EnterWindowMask;
}


/* vim: set sw=2 sts=2 : */
