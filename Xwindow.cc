#include "Xwindow.h"

Display *Xwindow::display = NULL;


Xwindow::Xwindow() {
  if (!display)
    display = XOpenDisplay(NULL);

  // TODO: replace w/XCreateWindow
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                               0, 0, 1, 1, /* pos, dimensions */
                               0 /* border width */, 0 /* border pix */, 0);
}


XEventHandler::XEventHandler(Xwindow &w) : window(w) { }


void XEventHandler::eventLoop() {
  bool process_events = true;
  while (process_events) {
    XEvent ev;
    XWindowEvent(window.getDisplay(), window.getWindow(), eventMask(), &ev);

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
