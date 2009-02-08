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

int main(int argc, char *argv[]) try  {

    unsigned int dblclk_tm, butpress;
    unsigned long dblclk0=0, sigexit = 0;

    OptParser *oParser;
    ConfigParser *cParser;
    Bar *wbar;
    XWin *wwin;
    XEvent ev;

    oParser = new OptParser(argc, argv);

    if( oParser->isSet("help") ) {
        oParser->printDescrip();
        delete oParser;
        return 0;
    }

    // Ignore Child exit status
    signal(SIGCHLD, SIG_IGN);

    // create wbar's window
    wwin = new XWin(50, 50, 50, 50);
    ImlibImage::InitImlib( wwin->get_display(), wwin->get_visual(), 
	    wwin->get_colormap(), wwin->get_drawable(), 2048*2048 );
    wwin->set_info( (char*)"wbar" );
    wwin->set_toolbar_properties(LAYER_ABOVE);

    // load wbar
    cParser = new ConfigParser( oParser->getArgument("config") );
    wbar = new Bar(wwin, cParser->getWidget(0));
    for(int i = 1; i < cParser->widgetCount(); i++)
        wbar->addIcon( cParser->getWidget(i), cParser->getCommand(i) );
    delete cParser;


    /* check if double clicking, ms time */
    dblclk_tm = oParser->isSet("dblclk") ? atoi(oParser->getArgument("dblclk")) : 0;
    butpress = oParser->isSet("bpress") ? 1 : 0;

    delete oParser;

    wwin->map();

    while( !sigexit ) {

        wwin->get_event( &ev );

        switch( ev.type ) {

            case EnterNotify:
                if(ev.xcrossing.mode != NotifyGrab && 
                        !(ev.xcrossing.state & Button1Mask))
                    wbar->refresh( ev.xcrossing.x );
                break;

            case LeaveNotify:
                if(ev.xcrossing.mode != NotifyUngrab && 
                        !(ev.xcrossing.state & Button1Mask))
                    wbar->refresh( -1 );
                break;

            case MotionNotify:
            case Expose:
                wbar->refresh( ev.xmotion.x );
                break;
        }
    
    }

    delete wwin;
    delete wbar;

    return 0;

} catch (const char *m) {

    std::cout << m << std::endl;
}
#if 0
int event_handler() {

    XEvent ev;

    while( !sigexit ) {

        wwin->get_event( &ev );

        switch(ev.type){

            case Expose:

                barra->refresh();
                break;

            case ButtonPress:

                switch(ev.xbutton.button){

                    case 1:
                        if(butpress!=0){
                            if((inum = barra->iconIndex(ev.xbutton.x))!=-1)
                                barra->iconDown(inum);
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

}
#endif
