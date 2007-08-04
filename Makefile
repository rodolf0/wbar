TARGET=wbar
CXXFLAGS=`imlib2-config --cflags` -Wall -O2 #-DCOS_ZOOM #-DLINEAR_TRASL -DNO_EXPAND #-DAVGFILTER
LDFLAGS=`imlib2-config --libs`  -Wl,-O2
PREFIX=/usr/share/wbar

	
sources= XWin.cc Icon.cc Bar.cc IconLoader.cc \
	 OptParser.cc SuperBar.cc SuperIcon.cc
objects= $(sources:.cc=.o) Main.o
headers= $(sources:.cc=.h) debug.h

all: $(TARGET) 

$(objects): $(headers) Makefile

$(TARGET): $(objects) 
	g++ $(LDFLAGS) -o $(@) $(objects)
	#strip $(@)

install: $(TARGET)
	if [ "`whoami`" != "root" ]; then \
		echo "You must be root to install"; \
		exit 1; \
	fi

	install -d $(PREFIX)
	awk '{if($$1 ~ /i:/ || ($$1 ~ /t:/ && NR<4)) print $$1" $(PREFIX)/"$$2; else print $$0;}' \
		./dot.wbar > $(PREFIX)/dot.wbar
	cp -a ./iconpack $(PREFIX)/iconpack

	ln -s -t $(PREFIX)/iconpack/wbar.nuvoux ../comic.ttf font.ttf
	ln -s -t $(PREFIX)/iconpack/wbar.ice ../comic.ttf font.ttf
	ln -s -t $(PREFIX)/iconpack/wbar.osx ../comic.ttf font.ttf
	
	install ./wbar /usr/bin

uninstall:
	if [ "`whoami`" != "root" ]; then \
		echo "You must be root to install"; \
		exit 1; \
	fi

	rm -rf $(PREFIX)
	rm -f /usr/bin/wbar


clean:
	rm -f *.o $(TARGET) 
