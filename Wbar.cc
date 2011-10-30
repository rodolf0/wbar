#include <iostream>
#include "OptionParser.h"
#include "ConfigReader.h"
#include "CanvasEngine.h"
#include "LayoutStrategy.h"
#include "Widget.h"
#include "Dock.h"

void showHelp() {
  std::cout << "wbar 2.0" << std::endl;
}


int main(int argc, char *argv[]) {

  OptionParser optparser(argc, argv);
  if (optparser.isset("help")) {
    showHelp();
    return 0;
  }

  //ConfigReader cfgreader(optparser.getString("config"));
  //WaveLayout layout(cfgreader.sections.size() - 1);
  WaveLayout layout(10,
                    optparser.getInt("size"),
                    optparser.getInt("nanim"),
                    optparser.getFloat("zoomf"),
                    optparser.getFloat("jumpf"));

  Xwindow window(layout.frameSize());
  window.setSkipPager();
  window.setSkipTaskbar();
  window.setSticky();
  window.setLayer(wlayer_above);
  window.setType(wtype_dock);

  Dock dock(layout);
  CanvasEngine::init(window);

  RectLayout dock_layout(layout.dockLayout());
  CanvasEngine::get().addImage("assets/dock.png", &dock_layout);

  typedef std::pair<Widget*, RectLayout*> LaidWidget;
  std::vector<LaidWidget> widgets;

  //for (size_t idx = 0; idx < cfgreader.sections.size(); idx++) {
  for (size_t idx = 0; idx < 10; idx++) {
    widgets.push_back(LaidWidget(new LauncherWidget(ExecuteCommand("")),
                                  new RectLayout(layout.widgetLayout(idx))));
    CanvasEngine::get().addImage("assets/firefox.png", widgets.back().second);
  }

  window.map();
  window.move(Point((Xwindow::screenSize().x - layout.frameSize().x) / 2,
                     Xwindow::screenSize().y - layout.frameSize().y));

  dock.eventLoop(window);
  //TODO: free widgets
  return 0;
}

/* vim: set sw=2 sts=2 : */
