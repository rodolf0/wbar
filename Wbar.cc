#include "OptionParser.h"
#include "ConfigReader.h"
#include "CanvasEngine.h"
#include "LayoutStrategy.h"
#include "Widget.h"
#include "Dock.h"

void showHelp();

int main(int argc, char *argv[]) {

  OptionParser optparser(argc, argv);
  if (optparser.getBool("help")) {
    showHelp();
    return 0;
  }

  ConfigReader cfgreader(optparser.getString("config"));

  WaveLayout layout(cfgreader.sections.size() - 1);
  Xwindow window; // (layout.frameSize());
  Dock dock(window,
            CanvasEngine::get().addImage("background.png", layout.dockLayout()),
            layout);

  std::vector<Widget*> widgets;
  for (ConfigReader::SectionIterator si = cfgreader.sections.begin();
       si != cfgreader.sections.end(); si++) {
    const size_t idx = std::distance(si, cfgreader.sections.begin());
    widgets.push_back(new LauncherWidget(
        CanvasEngine::get().addImage("widget.png", layout.widgetLayout(idx)),
        ExecuteCommand(si->getString("command"))));
  }

  //dock.eventLoop();
  return 0;
}

/* vim: set sw=2 sts=2 : */
