#ifndef _XWIN_H_
#define _XWIN_H_

/* X11 stuff */
#include <X11/Xlib.h>
#include <string>
#include "Image.h"

class Bar;

class XWin {
    public:
        friend class Bar;

        XWin(int x, int y, int w, int h);
        ~XWin();

        void map() const { 
            XMapWindow(display, window); 
        }

        void move_resize(int xx, int yy, int ww, int hh) {
            XMoveResizeWindow(display, window, xx, yy, ww, hh);
            x = xx; y = yy; w = ww; h = hh;
        }

        void lower() const {
            XLowerWindow(display, window);
        }

        void raise() const {
            XRaiseWindow(display, window);
        }

        int screen_width() const {
            return WidthOfScreen( DefaultScreenOfDisplay(display) );
        }

        int screen_height() const {
            return HeightOfScreen( DefaultScreenOfDisplay(display) );
        }

        Display *get_display() { return display; }
        Visual *get_visual() { return visual; }
        Colormap get_colormap() { return colormap; }
        Drawable get_drawable() { return window; }

#define LAYER_ABOVE 6
#define LAYER_BELOW 0
        void set_toolbar_properties(int wlayer);
        void setOverrideRedirection(Bool ovr = True);

        bool get_event(XEvent *ev) const;
        void set_info(char *name);

        ImlibImage& go_transparent();

    private:
        Display     *display;
        Visual      *visual;
        Window      window;

        ImlibImage  background;

        Colormap    colormap;

        Atom        delWindow;
        int         depth;
        int         eventMask;

        int x, y;
        int w, h;

        bool argb32_visual;
};

#endif /* _XWIN_H_ */
