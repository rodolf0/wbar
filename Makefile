TARGET=wbar
CXXFLAGS=`imlib2-config --cflags` -Wall -O2 -DDOCKWIN -DCOOLMACHINE_N_SHITCODE
LDFLAGS=`imlib2-config --libs`  -Wl,-O2
PREFIX=/usr/local/share/wbar

	
sources= XWin.cc Icon.cc Bar.cc IconLoader.cc \
	 OptParser.cc SuperBar.cc SuperIcon.cc
objects= $(sources:.cc=.o) Main.o
headers= $(sources:.cc=.h) debug.h

all: $(TARGET) 

$(objects): $(headers) Makefile

$(TARGET): $(objects) 
	g++ $(LDFLAGS) -o $(@) $(objects)
	strip $(@)

patch:
	patch < animation.patch

install: $(TARGET)
	if [ "`whoami`" != "root" ]; then \
		echo "You must be root to install"; \
		exit 1; \
	fi

	install -d $(PREFIX)
	awk '!/VeraBd/ {if($$1 == "i:") print "i: $(PREFIX)/"$$2; else print $$0} \
	    /VeraBd/ {print "t: $(PREFIX)/"$$2}' ./dot.wbar > $(PREFIX)/dot.wbar
	rsync -va ./wbar.icons/* $(PREFIX)/wbar.icons
	install ./wbar /usr/local/bin

config:
	if [ -f "$(HOME)/.wbar" -o -d "$(HOME)/.wbar.icons" ]; then \
		echo -en "Do you want to replace the existing config? "; \
		read recfg; \
		if [ "$$recfg" = "y" -o "$$recfg" = "Y" ]; then \
			awk '!/VeraBd/ {if($$1 == "i:") print "i: $(HOME)/."$$2; else print $$0} \
			/VeraBd/ {print "t: $(HOME)/."$$2}' ./dot.wbar > $(HOME)/.wbar; \
			rsync -va ./wbar.icons/* $(HOME)/.wbar.icons; \
		fi \
	fi

uninstall:
	if [ "`whoami`" != "root" ]; then \
		echo "You must be root to install"; \
		exit 1; \
	fi

	rm -rf $(PREFIX)
	rm -f /usr/local/bin/wbar


clean:
	rm -f *.o $(TARGET) 
