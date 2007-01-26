#ifndef _XWIN_H_
#define _XWIN_H_

/* X11 stuff */
#include <X11/Xlib.h>
#include <string>

class Bar;
class SuperBar;

class XWin{
    private:
	Display	    *display;
	Visual	    *visual;
	Window	    window;
	Atom	    delWindow;
	Colormap    colormap;
	int	    depth;
	int	    eventMask;

	int x, y;
	int w, h;

	friend class Bar;
	friend class SuperBar;
    public:

	XWin(int x, int y, int w, int h);
	~XWin();

	void selectInput(int mask);

	void mapWindow();
	void lowerWindow();
	void raiseWindow();
	bool nextEvent(XEvent *ev);
	void setName(char *name);

	int screenWidth() const;
	int screenHeight() const;
	Display *getDisplay();
	Visual	*getVisual();
	Window	getDrawable();
	Colormap getColormap();


	void moveNresize(int x, int y, int w, int h);

	void setOverrideRedirection(Bool ovr = True);
	void noDecorations();
	void setDesktopWindow();
	void setSticky();
	void skipTaskNPager();
	void bottomLayer();
};

#endif /* _XWIN_H_ */
