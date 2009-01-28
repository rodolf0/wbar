#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>
#include "OptParser.h"

#include "Image.h"
#include "XWin.h"
#include "Bar.h"

void corpshandler(int);

int main(int argc, char *argv[]) try  {

    OptParser oParser(argc, argv);
    ConfigParser *cParser;

    if( oParser.isSet("help") )
        oParser.printDescrip();

    signal(SIGCHLD, SIG_IGN);

    /* Variables *//*{{{*/

    unsigned int dblclk_tm, butpress;
    unsigned long dblclk0=0;
    int i, inum, vertbar;

    XEvent ev;

    XWin barwin(50, 50, 50, 50);
    Bar *barra;
    /*}}}*/

    
    barwin.setName((char*)"wbar");
    barwin.setDockWindow();
    barwin.skipTaskNPager();
    barwin.noDecorations();
    barwin.setSticky();
    barwin.bottomLayer();


    /* tell X what events we're intrested in */
    barwin.selectInput(PointerMotionMask | ExposureMask | ButtonPressMask /* |PropertyChangeMask */
	    | ButtonReleaseMask | LeaveWindowMask | EnterWindowMask);

    /* Image library set up */
    ImlibImage::InitImlib( barwin.getDisplay(), barwin.getVisual(), 
	    barwin.getColormap(), barwin.getDrawable(), 2048*2048 );


    /* check if double clicking, ms time */
    dblclk_tm = oParser.isSet("dblclk")?atoi(oParser.getArgument("dblclk")):0;
    butpress = oParser.isSet("bpress")?1:0;
    vertbar = oParser.isSet("vbar")?1:0;


    cParser = new ConfigParser( oParser.getArgument("config") );
	barra = new Bar(&barwin, cParser->getWidget(0));

    for(i = 1; i < cParser->widgetCount(); i++)
        barra->addIcon(cParser->getWidget(i), cParser->getCommand(i) );
    delete cParser;


    if( oParser.isSet("above-desk") ){
        barwin.mapWindow();
        barra->setPosition(oParser.getArgument("pos"));
    }else{
        barra->setPosition(oParser.getArgument("pos"));
        barwin.mapWindow();
    }

    barra->refresh();

    while( true ){

	barwin.nextEvent(&ev);
	switch(ev.type){

	    case Expose:
		barra->refresh();
		break;

	    case ButtonPress:
		switch(ev.xbutton.button){
		    case 1:
			if(butpress!=0){
			    if(!vertbar){
				if((inum = barra->iconIndex(ev.xbutton.x))!=-1)
				    barra->iconDown(inum);
			    }else{
				if((inum = barra->iconIndex(ev.xbutton.y))!=-1)
				    barra->iconDown(inum);
			    }
			}
			break;
		}
		break;

	    case ButtonRelease:
		switch(ev.xbutton.button){
		    case 3:/* Redraw Bar*/
			execvp(argv[0], argv);
			break;
		    case 1:/* Execute Program */
			if(!vertbar)
			    inum = barra->iconIndex(ev.xbutton.x);
			else
			    inum = barra->iconIndex(ev.xbutton.y);

			if(butpress!=0)
			    barra->iconUp(inum);

			/* Double click time 200 ms */
			if((ev.xbutton.time - dblclk0 <dblclk_tm || dblclk_tm==0) && inum != -1){
			    if(fork()==0)
				execlp("sh", "sh", "-c", barra->iconCommand(inum).c_str(), NULL);

			}else dblclk0 = ev.xbutton.time;
			break;
		}
		break;
	    /*}}}*/

	    case MotionNotify:
		if(!vertbar)
		    barra->refresh(ev.xmotion.x);
		else
		    barra->refresh(ev.xmotion.y);
		break;

	    case LeaveNotify:
		/* NotifyGrab && Ungrab r notified on B1 click*/
		if(ev.xcrossing.mode!=NotifyGrab && !(ev.xcrossing.state&Button1Mask))
		    barra->refresh();
		break;

	    case EnterNotify:
		if(ev.xcrossing.mode!=NotifyUngrab && !(ev.xcrossing.state&Button1Mask)){
		    if(!vertbar)
			barra->refresh(ev.xcrossing.x);
		    else
			barra->refresh(ev.xcrossing.y);
		}
		break;

	    default:
		break;
		
	}
    }
    /*}}}*/

    delete barra;
    return 0;
} catch (const char *m) {
    std::cout << m << std::endl;
}
