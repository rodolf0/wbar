TEST_CFLAGS=-O0 -gdb -Wall `pkg-config x11 evas evas-software-buffer --libs --cflags`
SOURCES=Wbar.cc Xwindow.cc LayoutStrategy.cc CanvasEngine.cc Dock.cc

all:
	clang++ -Weffc++ -fsyntax-only `pkg-config evas --cflags` $(SOURCES)

test-canvas:
	clang++ $(TEST_CFLAGS) TestCanvas.cc Xwindow.cc CanvasEngine.cc Geometry.cc
