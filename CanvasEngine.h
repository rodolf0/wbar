#ifndef _CANVASENGINE_
#define _CANVASENGINE_

#include <map>
#include <string>
#include <Evas.h>
#include "Xwindow.h"
#include "CanvasLayouts.h"


class CanvasEngine {
  public:

    static void init(Xwindow &frame);
    static CanvasEngine & get();

    virtual void addImage(const std::string &path, const Layout &l);
    virtual void render();

    virtual ~CanvasEngine();

  protected:
    CanvasEngine(Xwindow &frame);
    static CanvasEngine *instance;

    Evas *canvas;
    std::map<Evas_Object *, const Layout *> image_objects;
};

#endif /* _CANVASENGINE_ */

/* vim: set sw=2 sts=2 : */
