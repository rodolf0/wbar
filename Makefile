TARGET=wbar
CXXFLAGS=`imlib2-config --cflags` -Wall -O2 #-DCOS_ZOOM #-DLINEAR_TRASL -DNO_EXPAND #-DAVGFILTER
LDFLAGS=`imlib2-config --libs | sed s,@my_libs@,,`  -Wl,-O2 -lX11
#PREFIX=/usr/local
PREFIX=/tmp

	
sources= XWin.cc Icon.cc Bar.cc IconLoader.cc \
	 OptParser.cc SuperBar.cc SuperIcon.cc
objects= $(sources:.cc=.o) Main.o
headers= $(sources:.cc=.h) debug.h

all: $(TARGET) 

$(objects): $(headers) Makefile

$(TARGET): $(objects) 
	g++ $(LDFLAGS) -o $(@) $(objects)

install: $(TARGET)
	install -d $(PREFIX)/share/wbar $(PREFIX)/bin
	sed 's@iconpack@$(PREFIX)/share/wbar/iconpack@' dot.wbar > $(PREFIX)/share/wbar/dot.wbar
	cp -a iconpack $(PREFIX)/share/wbar/iconpack
	install wbar $(PREFIX)/bin/

uninstall:
	rm -rf $(PREFIX)/share/wbar
	rm -f $(PREFIX)/bin/wbar

clean:
	rm -f *.o $(TARGET) 
