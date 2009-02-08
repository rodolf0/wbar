#include <sys/select.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrender.h>
#include "XWin.h"

using namespace std;

XWin::XWin(int xx, int yy, int ww, int hh) : 
    background(1,1), eventMask(NoEventMask), x(xx), y(yy), w(ww), h(hh) {

    XVisualInfo vi, *xvi;
    XRenderPictFormat *format;
    XSetWindowAttributes wa;

    int render_event, render_error;
    int screen, nvi;

    if( !(display = XOpenDisplay(NULL)) )
        throw "Couldn't open display";

    screen = DefaultScreen(display);

    argb32_visual = false;

    // ARGB32 visual
    if( XRenderQueryExtension (display, &render_event, &render_error) ) {

        vi.screen = screen;
        depth = vi.depth = 32;
        vi.c_class = TrueColor;

        xvi = XGetVisualInfo(display, VisualScreenMask | VisualDepthMask | 
                VisualClassMask, &vi, &nvi);

        for(int i=0; i<nvi; i++) {
            format = XRenderFindVisualFormat(display, xvi[i].visual);

            if( format->type == PictTypeDirect && format->direct.alphaMask ) {
                visual = xvi[i].visual;
                argb32_visual = true;
                break;
            }
        }

        XFree(xvi);

        wa.colormap = colormap = XCreateColormap(display, RootWindow(display, screen),
                visual, AllocNone);

    } else {
    
        depth = DefaultDepth(display, screen);
        visual = DefaultVisual(display, screen);
        wa.colormap = colormap = DefaultColormap(display, screen);
    }

    wa.background_pixel = ScreenOfDisplay(display, screen)->black_pixel;
    wa.border_pixel = ScreenOfDisplay(display, screen)->white_pixel;
    wa.event_mask = eventMask = PointerMotionMask | ExposureMask | ButtonPressMask |
                                ButtonReleaseMask | LeaveWindowMask | EnterWindowMask;

    if( !(window = XCreateWindow(display, DefaultRootWindow(display), 
                        x, y, w, h, 0, depth, InputOutput, visual, CWBackPixel |
                        CWBorderPixel | CWEventMask | CWColormap, &wa)) )
        throw "Couldn't create window";


    /* prepare for delete window */
    delWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
    /* Set WM Protocols to report window delete event */
    XSetWMProtocols(display, window, &delWindow, 1);
}

XWin::~XWin(){
    XDestroyWindow(display, window);
    XFreeColormap(display, colormap);
    XCloseDisplay(display);
}

ImlibImage& XWin::go_transparent() {

    if( argb32_visual )
        background.colorClear(0, 0, 0, 0);
    else
        background = ImlibImage( DefaultRootWindow(display), x, y, w, h );

    return background;
}

bool XWin::get_event(XEvent *ev) const {

#ifndef CATCH_ALL_EVENTS
    int qlen;

    do{
        /* queue length is relate to my only window */
        if( (qlen = XQLength(display)) ){
            if( XCheckWindowEvent(display, window, eventMask, ev) == False ){
                /* process events we're not waiting for */
                XSync(display, True);
                continue;
            }
        }else
            XWindowEvent(display, window, eventMask, ev);
        
    }while( ev->type == MotionNotify && qlen > 1 );
#else
    XWindowEvent(display, window, eventMask, ev);
#endif

    return true;
}

void XWin::set_info(char *name){
    XTextProperty windowName;
    XClassHint ch;
    
    ch.res_name = name;
    ch.res_class = name;
    XStringListToTextProperty(&name, 1, &windowName);

    XSetWMName(display, window, &windowName);
    XSetClassHint(display, window, &ch);
}

/* Dont allow window manager to decorate / restack / remap / ... */
void XWin::setOverrideRedirection(Bool ovr){
    XSetWindowAttributes attr;
    
    attr.override_redirect = ovr;
    attr.background_pixmap = None; //Copy from back;
	
    XChangeWindowAttributes(display, window, CWOverrideRedirect | CWBackPixmap  , &attr);
}

void XWin::set_toolbar_properties(int wlayer) {
    Atom a, prop;
    long lprop[5] = { 0, 0, 0, 0, 0 };

    // set dock type (_NET_WM_WINDOW_TYPE_DESKTOP / NORMAL)
    a = XInternAtom(display, "_NET_WM_WINDOW_TYPE", True);
    prop = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", True);
    XChangeProperty(display, window, a, XA_ATOM, 32, PropModeReplace, (unsigned char *) &prop, 1);

    // no decorations
    a = XInternAtom(display, "_MOTIF_WM_HINTS", True);
    lprop[0] = 2;
    XChangeProperty(display, window, a, a, 32, PropModeReplace, (unsigned char *) lprop, 5);

    // set sticky
    a = XInternAtom(display, "_NET_WM_DESKTOP", True);
    lprop[0] = 0xFFFFFFFF;
    XChangeProperty(display, window, a, XA_CARDINAL, 32, PropModeAppend, (unsigned char *) lprop, 1);

    a = XInternAtom(display, "_NET_WM_STATE", True);
    prop = XInternAtom(display, "_NET_WM_STATE_STICKY", True);
    XChangeProperty(display, window, a, XA_ATOM, 32, PropModeAppend, (unsigned char *) &prop, 1);

    // skip pager
	prop = XInternAtom(display, "_NET_WM_STATE_SKIP_PAGER", True);
	XChangeProperty(display, window, a, XA_ATOM, 32, PropModeAppend, (unsigned char *) &prop, 1);

    // skip taskbar
	prop = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", True);
	XChangeProperty(display, window, a, XA_ATOM, 32, PropModeAppend, (unsigned char *) &prop, 1);

    // set win layer
    if( wlayer == LAYER_BELOW ) {
        prop = XInternAtom(display, "_NET_WM_STATE_BELOW", True);
        XChangeProperty(display, window, a, XA_ATOM, 32, PropModeAppend, (unsigned char *) &prop, 1);
    } else if( wlayer == LAYER_ABOVE ) {
        prop = XInternAtom(display, "_NET_WM_STATE_ABOVE", True);
        XChangeProperty(display, window, a, XA_ATOM, 32, PropModeAppend, (unsigned char *) &prop, 1);
    }
    a = XInternAtom(display, "_WIN_LAYER", True);
	XChangeProperty(display, window, a, XA_CARDINAL, 32, PropModeAppend, (unsigned char *) &wlayer, 1);

    // set opacity
	//prop = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", True);
    //wopacity = (int)(0xFFFFFFFF * opacity);
	//XChangeProperty(display, window, a, XA_CARDINAL, 32, PropModeAppend, (unsigned char *) &wopacity, 1);
}
