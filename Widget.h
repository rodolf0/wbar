#ifndef _WIDGET_
#define _WIDGET_

#include "CanvasEngine.h"
#include "Command.h"

class Widget {
  public:
    virtual ~Widget() = 0;
    virtual void action() const = 0;
};


class LauncherWidget : public Widget {
  public:
    LauncherWidget(const Command &comm);
    void action() const;
};

#endif /* _WIDGET_ */

/* vim: set sw=2 sts=2 : */
