#include "OptionParser.h"
#include "ConfigReader.h"
#include "CanvasEngine.h"
#include "LayoutStrategy.h"
#include "Widget.h"
#include "Dock.h"

void showHelp() {
}

int main(int argc, char *argv[]) {

  //OptionParser optparser(argc, argv);
  //if (optparser.getBool("help")) {
    //showHelp();
    //return 0;
  //}

  //ConfigReader cfgreader(optparser.getString("config"));

  //WaveLayout layout(cfgreader.sections.size() - 1);
  WaveLayout layout(10);
  Xwindow window(layout.frameSize());
  CanvasEngine::init(window);

  typedef std::pair<Widget*, RectLayout*> LayedWidget;
  std::vector<LayedWidget> widgets;

  //for (size_t idx = 0; idx < cfgreader.sections.size(); idx++) {
  for (size_t idx = 0; idx < 10; idx++) {
    widgets.push_back(LayedWidget(new LauncherWidget(ExecuteCommand("")),
                                  new RectLayout(layout.widgetLayout(idx))));
    CanvasEngine::get().addImage("assets/firefox.png", widgets.back().second);
  }

  //RectLayout dock_layout(layout.dockLayout());
  //CanvasEngine::get().addImage("assets/dock.png", &dock_layout);
  Dock dock(layout);

  window.map();
  window.move(Point(0, 0));
  dock.eventLoop(window);

  //TODO: free widgets
  return 0;
}

/* vim: set sw=2 sts=2 : */
