TARGET=Wbar

CXXFLAGS=\
	-include cstddef \
	-O0 -ggdb -Wall -Weffc++ \
	`pkg-config x11 xrender evas --cflags` \
	#-DDISCARD_MOUSE_MOVES

LDFLAGS=`pkg-config x11 xrender evas --libs` -Wl,-O0

OBJECTS = \
	Wbar.o \
	OptionParser.o \
	ConfigReader.o \
	CanvasEngine.o \
	LayoutStrategy.o \
	Geometry.o \
	Xwindow.o


$(TARGET): $(OBJECTS)
	clang++ $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.cc
	clang++ $(CXXFLAGS) -c $^

clean:
	rm -f *.o $(TARGET)


TEST_CFLAGS=-O0 -gdb -Wall `pkg-config x11 xrender evas evas-software-buffer --libs --cflags`
test-canvas:
	clang++ $(TEST_CFLAGS) TestCanvas.cc Xwindow.cc CanvasEngine.cc Geometry.cc CanvasLayouts.cc

test-options:
	clang++ $(TEST_CFLAGS) TestOP.cc OptionParser.cc
