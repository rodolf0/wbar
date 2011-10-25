TARGET=Wbar

CXXFLAGS=-include cstddef -O0 -ggdb -Wall -Weffc++ `pkg-config x11 xrender evas --cflags`
LDFLAGS=`pkg-config x11 xrender evas --libs`

OBJECTS = \
	CanvasEngine.o \
	CanvasLayouts.o \
	Command.o \
	Dock.o \
	Geometry.o \
	LayoutStrategy.o \
	Wbar.o \
	Widget.o \
	Xwindow.o \


$(TARGET): $(OBJECTS)
	clang++ $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.cc
	clang++ $(CXXFLAGS) -c $^


check:
	clang++ -fsyntax-only -Wall -Weffc++ `pkg-config x11 xrender evas --cflags` $(OBJECTS:.o=.cc)


clean:
	rm -f *.o $(TARGET)


TEST_CFLAGS=-O0 -gdb -Wall `pkg-config x11 xrender evas evas-software-buffer --libs --cflags`
test-canvas:
	clang++ $(TEST_CFLAGS) TestCanvas.cc Xwindow.cc CanvasEngine.cc Geometry.cc CanvasLayouts.cc
