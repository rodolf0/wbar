#ifndef _CANVASENGINE_
#define _CANVASENGINE_

#include <list>
#include <string>
#include <Evas.h>
#include "Xwindow.h"
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


class CanvasEngine {
  public:

    static void init(Xwindow &frame);
    static CanvasEngine & get();

    virtual void addRectWidget(const std::string &path, const Rect &r);
    virtual void render();
    virtual void resize(const Size &s);

    virtual ~CanvasEngine();

  protected:
    CanvasEngine(const CanvasEngine &);
    CanvasEngine & operator=(const CanvasEngine &);

    CanvasEngine(Xwindow &frame);
    static CanvasEngine *instance;

    Evas *canvas;
    typedef std::pair<Evas_Object *, const Layout *> widget_t;
    std::list<widget_t> widgets;
};

#endif /* _CANVASENGINE_ */

/* vim: set sw=2 sts=2 : */
