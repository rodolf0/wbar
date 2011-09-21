#include <iostream>
#include "Xwindow.h"
#include "CanvasEngine.h"
#include "Geometry.h"


class Test : public XEventHandler {
  public:

    void onExposure(const XExposeEvent &e) {
      CanvasEngine::get().render();
    }
    void onMouseMove(const XMotionEvent &e) {
      std::cout << "mm. x=" << e.x << ", y=" << e.y << std::endl;
    }
    void onMouseDown(const XButtonEvent &e) {
      std::cout << "mouse down" << std::endl;
    }
    void onMouseUp(const XButtonEvent &e) {}
    void onMouseEnter(const XCrossingEvent &e) {}
    void onMouseLeave(const XCrossingEvent &e) {}
};



int main(int argc, char *argv[]) {

  try {
    Xwindow w(Point(20, 20), Size(300, 100));
    RectLayout l(0, 0, 300, 100);

    CanvasEngine::init(w);

    CanvasEngine::get().addImage("assets/dock.png", l);

    w.map();

    Test t;

    t.eventLoop(w);

  } catch (const char *c) {
    std::cout << c << std::endl;
  } catch (int32_t ret) {
    std::cout << ret << std::endl;
  }

  return 0;
}

/* vim: set sw=2 sts=2 : */
