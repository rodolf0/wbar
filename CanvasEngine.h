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

    static CanvasEngine & get();
    static void init();

    virtual Image & addImage(const std::string &path, const Layout &l);

    virtual ~CanvasEngine();

  private:
    CanvasEngine(Xwindow &canvas);
    static CanvasEngine &canvas_engine;
    std::map<std::string, Image> image_objects;
};


#endif /* _CANVASENGINE_ */

/* vim: set sw=2 sts=2 : */
