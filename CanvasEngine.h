#ifndef _CANVASENGINE_
#define _CANVASENGINE_

#include <list>
#include <string>
#include <Evas.h>
#include "Xwindow.h"
#include "Geometry.h"


///////////////////// Layouts ////////////////////////////


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

class Layout3D : public Layout {
  public:
    Layout3D(const Rect3D & r);
    void transform(Evas_Object *img) const;
  protected:
    const Rect3D &layout;
};


/////////////////////// Canvas Widget and Engine //////////////////////


class CanvasWidget {
  public:
    virtual ~CanvasWidget();

    void setFrame(const Border &b);

  protected:
    friend class CanvasEngine;

    CanvasWidget(Evas_Object *img, const Layout &l);
    void render() const;

    Evas_Object *image;
    const Layout *layout;
};


class CanvasEngine {
  public:

    static void init(Xwindow &frame);
    static CanvasEngine & get();

    virtual CanvasWidget & addWidget(const std::string &path, const Layout &l);
    virtual void render() const;
    virtual void resize(const Size &s);

    virtual ~CanvasEngine();

  protected:

    CanvasEngine(const CanvasEngine &);
    CanvasEngine & operator=(const CanvasEngine &);

    CanvasEngine(Xwindow &frame);
    static CanvasEngine *instance;

    Evas *canvas;
    std::list<CanvasWidget *> widgets;
};

#endif /* _CANVASENGINE_ */

/* vim: set sw=2 sts=2 : */
