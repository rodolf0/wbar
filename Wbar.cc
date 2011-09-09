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

  EvasEngine canvasEngine;
  WaveLayout layoutStrategy;
  Dock dock(canvasEngine, layoutStrategy);

  for (ConfigReader::SectionIterator si = cfgreader.sections.begin();
       si != cfgreader.sections.end(); si++) {
    Widget *w = new LauncherWidget(ExecuteCommand(si->getString("command")));
    dock.addWidget(*w);
  }

  dock.eventLoop();

  return 0;
}

/* vim: set sw=2 sts=2 : */
