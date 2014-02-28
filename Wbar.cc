#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "OptionParser.h"
#include "ConfigReader.h"
#include "CanvasEngine.h"
#include "LayoutStrategy.h"

class Wbar : public XEventHandler {
private:
  Wbar(const Wbar &);
  Wbar &operator=(const Wbar &);

  ConfigReader cfgreader;
  Xwindow window;
  WaveLayout *layout;

  XButtonEvent mouse_position;

public:
  Wbar(const OptionParser &optparser)
      : cfgreader(optparser.getString("config")), window(Size(10, 10)),
        layout(new WaveLayout(
            optparser.getInt("size"), optparser.getInt("nanim"),
            optparser.getFloat("zoomf"), optparser.getFloat("jumpf"))) {

    CanvasEngine::init(window);

    // if (cfgreader.get("Dock").get("layout") == "Wave") {
    //  layout = new WaveLayout();
    //}

    CanvasWidget &dock = CanvasEngine::get().addWidget(
        cfgreader.get("Dock").get("face"), RectLayout(layout->dockLayout()));
    dock.setFrame(Border(5, 5, 5, 5));

    for (const auto &elem : cfgreader) {
      if (elem.get("type") == "LauncherWidget") {
        CanvasEngine::get().addWidget(elem.get("face"),
                                      RectLayout(layout->addWidget()));
      }
    }

    window.resize(layout->frameSize());
    CanvasEngine::get().resize(layout->frameSize());

    window.setSkipPager();
    window.setSkipTaskbar();
    window.setSticky();
    window.setType(wtype_dock);
    window.setLayer(wlayer_above);
    window.decorationsOff();
    window.map();
    window.move(Point((Xwindow::screenSize().x - layout->frameSize().x) / 2,
                      Xwindow::screenSize().y - layout->frameSize().y));
    eventLoop(window);
  }

  ~Wbar() { delete layout; }

  void onExposure(const XExposeEvent &) override {
    CanvasEngine::get().render();
  }

  void onMouseMove(const XMotionEvent &e) override {
    const Point p(e.x, e.y);
    if (layout->atHoverZone(p)) {
      layout->focus(p);
    } else {
      layout->unfocus();
    }
    CanvasEngine::get().render();
  }

  void onMouseDown(const XButtonEvent &e) override { mouse_position = e; }

  void onMouseUp(const XButtonEvent &e) override {
    if (mouse_position.x == e.x && mouse_position.y == e.y) {
      int idx = layout->widgetAt(Point(e.x, e.y));
      for (const auto &elem : cfgreader) {
        if (elem.get("type") == "LauncherWidget" && idx-- == 0) {
          if (!fork()) {
            execl("/bin/sh", "/bin/sh", "-c", elem.get("command").c_str(),
                  NULL);
            _exit(1);
          } else
            break;
        }
      }
    }
  }

  void onMouseEnter(const XCrossingEvent &e) override {
    layout->focus(Point(e.x, e.y));
    CanvasEngine::get().render();
  }

  void onMouseLeave(const XCrossingEvent &) override {
    layout->unfocus();
    CanvasEngine::get().render();
  }
};

void showHelp() { std::cout << "wbar 2.0" << std::endl; }

int main(int argc, char *argv[]) {
  try {
    OptionParser optparser(argc, argv);
    if (optparser.isset("help")) {
      showHelp();
    } else {
      struct sigaction sa;
      sa.sa_flags = SA_NOCLDSTOP;
      sa.sa_handler = SIG_IGN;
      sigaction(SIGCHLD, &sa, nullptr);
      Wbar wbar(optparser);
    }
  }
  catch (const char *e) {
    std::cout << e << std::endl;
  }
  return 0;
}

/* vim: set sw=2 sts=2 : */
