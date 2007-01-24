#include <sys/select.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "XWin.h"

using namespace std;

XWin::XWin(int xx, int yy, int ww, int hh) : 
    eventMask(NoEventMask), x(xx), y(yy), w(ww), h(hh){

    /* if(threads) XInitThreads() */
    int defScreen;

    if( !(display = XOpenDisplay(NULL)) )
	throw "Couldn't open display";

    defScreen = DefaultScreen(display);
    if( !(visual = DefaultVisual(display, defScreen)) )
	throw "Couldn't get visual";
    if( !(colormap = DefaultColormap(display, defScreen)) )
	throw "Couln't get colormap";
    if( !(depth = DefaultDepth(display, defScreen)) )
	throw "Couldn't get depth";
    if( !(window = XCreateSimpleWindow(display, 
	    DefaultRootWindow(display), x, y, w, h, 0, 0, 0)) )
	throw "Couldn't create window";

    /* prepare for delete window */
    delWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
    /* Set WM Protocols to report window delete event */
    XSetWMProtocols(display, window, &delWindow, 1);

    XClassHint ch = {"wbar", "wbar"};
    XSetClassHint(display, window, &ch);
}

XWin::~XWin(){
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

void XWin::selectInput(int ev_mask){
    eventMask = ev_mask;
    XSelectInput(display, window, ev_mask);
}

void XWin::lowerWindow(){
    XLowerWindow(display, window);
}

void XWin::raiseWindow(){
    XRaiseWindow(display, window);
}

void XWin::mapWindow(){
    XMapWindow(display, window);
}

#if 0
/* nextEvent for normal apps wich wouldn't fast response *//*{{{*/
/* returns false if timeout */
bool XWin::nextEvent(XEvent *ev, int msec){
    struct timeval timeout;
    fd_set rset;

    XSync(display, False);
    if (XPending(display)) {
	XNextEvent(display, ev);
    }else{
	timeout.tv_sec = msec / 1000;
	timeout.tv_usec = (msec % 1000) * 1000;

	FD_ZERO(&rset);
	FD_SET(ConnectionNumber(display), &rset);

	if (select(ConnectionNumber(display)+1, &rset, NULL, NULL, &timeout) > 0) 
	    XNextEvent(display, ev);
	else
	    /* timeout */
	    return false;
    }

    if(ev->type == ClientMessage)
	if(ev->xclient.data.l[0] == (long)delWindow){
	    XDestroyWindow(display, window);
	    XFlush(display);
	}

    return true;
}
/*}}}*/
#endif

bool XWin::nextEvent(XEvent *ev){
    /* Dont block cause pointer-motion events acumulate
     * and we need to discard them to keep interactive
     */
	// This is the old method/*{{{*/
# if 0
    /* The sense of this is: process important all important
     * events. Once that's done flush posibly acumulated 
     * pointer motion events */

    /* Check for the events we're interested in except
     * for pointer motion notifications */
    if( XCheckWindowEvent(display, window, 
               eventMask & ~PointerMotionMask, ev) )
       return true;
    else{
       /* Wait for server to process events 
        * & discards events in queue (1) */
       XSync(display, True);
       /* No ev in queue => sleeps 
        * once we procesed all important events and discarded
        * acumulated trivial events wait fore some event */
       XWindowEvent(display, window, eventMask, ev);
       return true;
    }
#endif
/*}}}*/

    int qlen;

    do{
	/* queue length is relate to my only window */
	if( (qlen = XQLength(display)) ){
	    if( XCheckWindowEvent(display, window, eventMask, ev) == False ){
		/* process events we're not waiting for */
		XSync(display, True);
		XWindowEvent(display, window, eventMask, ev);
	    }
	}else
	    XWindowEvent(display, window, eventMask, ev);
	
    }while( ev->type == MotionNotify && qlen > 1 );

    return true;
}

void XWin::setName(char *name){
    XTextProperty windowName;
    XStringListToTextProperty(&name, 1, &windowName);

    XSetWMName(display, window, &windowName);
}

void XWin::moveNresize(int x, int y, int w, int h){
    XMoveResizeWindow(display, window, x, y, w, h);
}

int XWin::screenWidth() const{
    return WidthOfScreen(DefaultScreenOfDisplay(display));
}

int XWin::screenHeight()const{
    return HeightOfScreen(DefaultScreenOfDisplay(display));
}

Display *XWin::getDisplay(){ return display; }
Visual *XWin::getVisual(){ return visual; }
Colormap XWin::getColormap(){ return colormap; }
Drawable XWin::getDrawable(){ return window; }

/* Dont allow window manager to decorate / restack / remap / ... */
void XWin::setOverrideRedirection(Bool ovr){
    XSetWindowAttributes attr;
    
    attr.override_redirect = ovr;
    attr.background_pixmap = None; //Copy from back;
	
    XChangeWindowAttributes(display, window, CWOverrideRedirect | CWBackPixmap  , &attr);
}

void XWin::setDockWindow(){
    Atom a = XInternAtom(display, "_NET_WM_WINDOW_TYPE", True);

    if (a != None) {
	Atom prop = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", True);
	// For XFCE NORMAL keeps window from going under desktop
	//Atom prop = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NORMAL", True);
	XChangeProperty(display, window, a, XA_ATOM, 32, PropModeReplace, (unsigned char *) &prop, 1);
    }
}

void XWin::noDecorations(){
    Atom a = XInternAtom(display, "_MOTIF_WM_HINTS", True);

    if (a != None) {
	long prop[5] = { 2, 0, 0, 0, 0 };
	XChangeProperty(display, window, a, a, 32, PropModeReplace, (unsigned char *) prop, 5);
    }
}

void XWin::setSticky(){
    Atom a = XInternAtom(display, "_NET_WM_DESKTOP", True);

    if (a != None) {
	long prop = 0xFFFFFFFF;
	XChangeProperty(display, window, a, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &prop, 1);
    }

    /* PHK: Add EWMH hint STICKY to _NET_WM_STATE */
    a = XInternAtom(display, "_NET_WM_STATE", True);
    if (a != None) {
	Atom prop = XInternAtom(display, "_NET_WM_STATE_STICKY", True);
	XChangeProperty(display, window, a, XA_ATOM, 32, PropModeAppend, (unsigned char *) &prop, 1);
    }
}

void XWin::skipTaskNPager(){

    /* PHK: Add EWMH hint SKIP_TASKBAR to _NET_WM_STATE */
    Atom a = XInternAtom(display, "_NET_WM_STATE", True);
    if (a != None) {
	Atom prop = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", True);
	XChangeProperty(display, window, a, XA_ATOM, 32, PropModeAppend, (unsigned char *) &prop, 1);
    }

    /* PHK: Add EWMH hint SKIP_PAGER to _NET_WM_STATE */
    a = XInternAtom(display, "_NET_WM_STATE", True);
    if (a != None) {
	Atom prop = XInternAtom(display, "_NET_WM_STATE_SKIP_PAGER", True);
	XChangeProperty(display, window, a, XA_ATOM, 32, PropModeAppend, (unsigned char *) &prop, 1);
    }
}

void XWin::bottomLayer(){
    /* make sure the layer is on the bottom */
    Atom a = XInternAtom(display, "_WIN_LAYER", True);
    if (a != None) {
	long prop = 0;
	XChangeProperty(display, window, a, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &prop, 1);
    }
    /* PHK: also append EWMH hint for BELOW also */
    a = XInternAtom(display, "_NET_WM_STATE", True);
    if (a != None) {
	Atom prop = XInternAtom(display, "_NET_WM_STATE_BELOW", True);
	XChangeProperty(display, window, a, XA_ATOM, 32, PropModeAppend, (unsigned char *) &prop, 1);
    }
}
