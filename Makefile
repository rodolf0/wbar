TARGET=wbar
CXXFLAGS=`imlib2-config --cflags` -Wall -O2 #-DCOS_ZOOM #-DLINEAR_TRASL #-DNO_EXPAND #-DAVGFILTER
LDFLAGS=`imlib2-config --libs` -lXrender -Wl,-O2
PREFIX=/usr/share/wbar

	
sources= XWin.cc Icon.cc Bar.cc OptParser.cc
objects= $(sources:.cc=.o) Main.o
headers= $(sources:.cc=.h) Image.h

all: $(TARGET) 

$(objects): $(headers) Makefile

$(TARGET): $(objects) 
	g++ $(LDFLAGS) -o $(@) $(objects)
	#strip $(@)

install: $(TARGET)
	install -d $(PREFIX)
	awk '{if($$1 ~ /i:/ || ($$1 ~ /t:/ && NR<4)) print $$1" $(PREFIX)/"$$2; else print $$0;}' \
		./dot.wbar > $(PREFIX)/dot.wbar
	cp -a ./iconpack $(PREFIX)/iconpack

	ln -fs $(PREFIX)/iconpack/comic.ttf $(PREFIX)/iconpack/wbar.nuvoux/font.ttf
	ln -fs $(PREFIX)/iconpack/comic.ttf $(PREFIX)/iconpack/wbar.ice/font.ttf
	ln -fs $(PREFIX)/iconpack/comic.ttf $(PREFIX)/iconpack/wbar.osx/font.ttf
	
	install ./wbar /usr/bin

uninstall:
	rm -rf $(PREFIX)
	rm -f /usr/bin/wbar


clean:
	rm -f *.o $(TARGET) 
