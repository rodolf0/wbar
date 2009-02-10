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
    Image::Init( wwin->get_display(), wwin->get_visual(), 
	    wwin->get_colormap(), wwin->get_drawable(), 2048*2048 );

    // load wbar
    cParser = new ConfigParser( oParser->getArgument("config") );
    wbar = new Bar(wwin, cParser->getWidget(0));
    for(int i = 1; i < cParser->widgetCount(); i++)
        wbar->addIcon( cParser->getWidget(i), cParser->getCommand(i) );
    delete cParser;

    oParser->configure(wbar);

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
