#ifndef _CANVASENGINE_
#define _CANVASENGINE_

#include <map>
#include <string>
#include <Evas.h>
#include "Xwindow.h"


class Image {
  public:
    Layout &layout;

  private:
    Image(Layout &l);
};


class CanvasEngine {
  public:

    static void init(Xwindow &frame);
    static CanvasEngine & get();

    virtual Image & addImage(const std::string &path, const Layout &l);
    virtual void render();

    virtual ~CanvasEngine();

  protected:
    CanvasEngine(Xwindow &frame);
    static CanvasEngine *instance;

    Evas *canvas;

    std::map<std::string, Image> image_objects;
};


#endif /* _CANVASENGINE_ */

/* vim: set sw=2 sts=2 : */
