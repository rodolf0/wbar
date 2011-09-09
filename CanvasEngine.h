#ifndef _CANVASENGINE_
#define _CANVASENGINE_

#include <Evas.h>
#include <Imlib2.h>

class CanvasEngine {
  public:
    class Image {
      public:
    };

    virtual ~CanvasEngine() = 0;
};


class Imlib2Engine : public CanvasEngine {
  public:

    class Imlib2Image : public Image {
      public:
        Imlib2Image scale() const;
        void render();
    };

    Imlib2Engine();
    Imlib2Image createImage();
};


class EvasEngine : public CanvasEngine {
};

#endif /* _CANVASENGINE_ */

/* vim: set sw=2 sts=2 : */
