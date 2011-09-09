#include "Xwindow.h"

void Xwindow::eventLoop() {
  bool process_events = true;
  while (process_events) {
    XEvent ev;
    XWindowEvent(this->d, this->w, this->ev_mask, &ev);

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

/* vim: set sw=2 sts=2 : */
