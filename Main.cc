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
    int inum = -1;

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
    wbar->set_focus(0);

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

            case ButtonPress:
                if( ev.xbutton.button == 1 && butpress != 0 )
                    if( (inum = wbar->icon_index(ev.xbutton.x)) !=-1 )
                        wbar->icon_pressure(inum, 2);

                break;

            case ButtonRelease:
                switch(ev.xbutton.button){
                    case 3:
                        execvp(argv[0], argv);
                        break;
                    
                    case 1:
                        if(butpress!=0)
                            wbar->icon_pressure(inum, -2);

                        inum = wbar->icon_index(ev.xbutton.x);

                        /* Double click time 200 ms */
                        if( (ev.xbutton.time - dblclk0 <dblclk_tm || dblclk_tm==0) && inum != -1 ) {
                            if(fork()==0)
                                execlp("sh", "sh", "-c", wbar->icon_cmd(inum).c_str(), NULL);
                        } else
                            dblclk0 = ev.xbutton.time;

                    break;
                }
                break;
        }
    
    }

    delete wwin;
    delete wbar;

    return 0;

} catch (const char *m) {

    std::cout << m << std::endl;
}
