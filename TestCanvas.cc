#include <iostream>
#include "Xwindow.h"
#include "CanvasEngine.h"


class Test : public XEventHandler {
  public:
    Rect &rect;

    Test(Rect &r) : rect(r) {}

    void onExposure(const XExposeEvent &e) {
      CanvasEngine::get().render();
    }
    void onMouseMove(const XMotionEvent &e) {
      rect.x += 1;
      CanvasEngine::get().render();
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
    Xwindow w(Size(300, 100));
    Rect r(100, 0, 48, 48);
    RectLayout rl(r);

    CanvasEngine::init(w);
    CanvasEngine::get().addImage("assets/firefox.png", rl);

    w.map();
    w.move(Point(10, 10));

    Test(r).eventLoop(w);

  } catch (const char *c) {
    std::cout << c << std::endl;
  } catch (int32_t ret) {
    std::cout << ret << std::endl;
  }

  return 0;
}

/* vim: set sw=2 sts=2 : */
