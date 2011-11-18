#include <iostream>
#include "OptionParser.h"
#include "ConfigReader.h"
#include "CanvasEngine.h"
#include "LayoutStrategy.h"

class Wbar : public XEventHandler {
  private:

    Wbar(const Wbar &);
    Wbar & operator=(const Wbar &);

    ConfigReader cfgreader;
    Xwindow window;
    WaveLayout *layout;

  public:

    Wbar(const OptionParser &optparser) :
        cfgreader(optparser.getString("config")),
        window(Size(10, 10)),
        layout(new WaveLayout(optparser.getInt("size"),
                              optparser.getInt("nanim"),
                              optparser.getFloat("zoomf"),
                              optparser.getFloat("jumpf"))) {

      CanvasEngine::init(window);

      //if (cfgreader.get("Dock").get("layout") == "Wave") {
      //}

      CanvasWidget &dock = CanvasEngine::get().addWidget(
          cfgreader.get("Dock").get("face"),
          RectLayout(layout->dockLayout()));
      dock.setFrame(Border(5, 5, 5, 5));

      for (std::list<ConfigReader::Section>::const_iterator section =
           cfgreader.begin(); section != cfgreader.end(); section++) {
        if (section->get("type") == "LauncherWidget") {
          CanvasEngine::get().addWidget(section->get("face"),
                                        RectLayout(layout->addWidget()));
        }
      }

      window.resize(layout->frameSize());
      CanvasEngine::get().resize(layout->frameSize());

      window.setSkipPager();
      window.setSkipTaskbar();
      window.setSticky();
      window.setLayer(wlayer_above);
      window.setType(wtype_dock);
      window.map();
      window.move(Point((Xwindow::screenSize().x - layout->frameSize().x) / 2,
                         Xwindow::screenSize().y - layout->frameSize().y));
      eventLoop(window);
    }

    ~Wbar() {
      delete layout;
    }

    void onExposure(const XExposeEvent &e) {
      CanvasEngine::get().render();
    }

    void onMouseMove(const XMotionEvent &e) {
      const Point p(e.x, e.y);
      if (layout->atHoverZone(p)) {
        layout->focus(p);
      } else {
        layout->unfocus();
      }
      CanvasEngine::get().render();
    }

    void onMouseDown(const XButtonEvent &e) {
      int idx = layout->widgetAt(Point(e.x, e.y));
    }

    void onMouseUp(const XButtonEvent &e) {
      int idx = layout->widgetAt(Point(e.x, e.y));
      std::cout << "Widget clicked: " << idx << std::endl;
    }

    void onMouseEnter(const XCrossingEvent &e) {
      layout->focus(Point(e.x, e.y));
      CanvasEngine::get().render();
    }

    void onMouseLeave(const XCrossingEvent &e) {
      layout->unfocus();
      CanvasEngine::get().render();
    }
};



void showHelp() {
  std::cout << "wbar 2.0" << std::endl;
}


int main(int argc, char *argv[]) {
  try {
    OptionParser optparser(argc, argv);
    if (optparser.isset("help")) {
      showHelp();
    } else {
      Wbar wbar(optparser);
    }
  } catch (const char *e) {
    std::cout << e << std::endl;
  }
  return 0;
}

/* vim: set sw=2 sts=2 : */
