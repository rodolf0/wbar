#ifndef _CANVASLAYOUTS_
#define _CANVASLAYOUTS_

#include <Evas.h>
#include "Geometry.h"

class Layout {
  public:
    virtual ~Layout() = 0;
    virtual void transform(Evas_Object *img) const = 0;
};


class RectLayout : public Layout {
  public:
    RectLayout(const Rect &r);
    void transform(Evas_Object *img) const;

  protected:
    const Rect &layout;
};


#endif /* _CANVASLAYOUTS_ */

/* vim: set sw=2 sts=2 : */
