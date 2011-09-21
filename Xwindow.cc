#include "Xwindow.h"

Display *Xwindow::display = NULL;


Xwindow::Xwindow(const Point &position, const Size &size) :
    position(position), dimensions(size) {
  if (!display)
    display = XOpenDisplay(NULL);

  // TODO: replace w/XCreateWindow
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                               position.x, position.y, dimensions.x, dimensions.y,
                               0 /* border width */, 0 /* border pix */, 0);
}


Xwindow::~Xwindow() {}


Display * Xwindow::getDisplay() {
  return display;
}


Window Xwindow::getWindow() const {
  return window;
}


void Xwindow::setGeometry(int x, int y, int w, int h) {
  position.x = x; position.y = y;
  dimensions.x = w; dimensions.y = w;
}

void Xwindow::map() const {
  XMapWindow(display, window);
}

size_t Xwindow::width() const {
  return dimensions.x;
}

size_t Xwindow::height() const {
  return dimensions.y;
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
