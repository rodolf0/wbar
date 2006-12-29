TARGET=wbar
CXXFLAGS=`imlib2-config --cflags` -Wall -O2 -DDOCKWIN -DCOOLMACHINE_N_SHITCODE -DAVGFILTER
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

patch-anim1:
	patch < animation.patch

patch-anim2:
	patch < animation2.patch

install: $(TARGET)
	if [ "`whoami`" != "root" ]; then \
		echo "You must be root to install"; \
		exit 1; \
	fi

	install -d $(PREFIX)
	awk '{if($$1 ~ /i:/ || ($$1 ~ /t:/ && NR<4)) print $$1" $(PREFIX)/"$$2; else print $$0;}' \
		./dot.wbar > $(PREFIX)/dot.wbar
	cp -a ./wbar.icons $(PREFIX)/wbar.icons
	install ./wbar /usr/local/bin

config:
	if [ -f "$(HOME)/.wbar" -o -d "$(HOME)/.wbar.icons" ]; then \
		echo -en "Do you want to replace the existing config? "; \
		read recfg; \
		if [ "$$recfg" = "y" -o "$$recfg" = "Y" ]; then \
			awk '{if($$1 ~ /i:/ || ($$1 ~ /t:/ && NR<4)) print $$1" $(HOME)/"$$2; else print $$0;}' \
			    ./dot.wbar > $(HOME)/dot.wbar; \
			cp -a ./wbar.icons $(HOME)/.wbar.icons; \
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
