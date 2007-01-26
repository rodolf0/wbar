#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include "XWin.h"
#include "ImgWrap.h"
#include "OptParser.h"
#include "IconLoader.h"
#include "SuperBar.h"

using namespace std;

void corpshandler(int);

int main(int argc, char **argv) try{

    /* Variables *//*{{{*/
    struct sigaction sigh;

    unsigned int dblclk_tm, butpress;
    unsigned long dblclk0=0;
    int inum, vertbar;

    string image, command, text;

    XEvent ev;

    XWin barwin(50, 50, 50, 50);
    OptParser optparser;
    Bar *barra;
    /*}}}*/

    /* Parse command line options *//*{{{*/
    optparser.addOption("help", 0);
    optparser.addOption("config", 1, "-");
    optparser.addOption("above-desk", 0);
    optparser.addOption("isize", 1, "32");
    optparser.addOption("idist", 1, "1");
    optparser.addOption("zoomf", 1, "1.8");
    optparser.addOption("jumpf", 1, "1");
    optparser.addOption("pos", 1, "bottom");
    optparser.addOption("balfa", 1, "-1");
    optparser.addOption("falfa", 1, "-1");
    optparser.addOption("dblclk", 1, "200");
    optparser.addOption("bpress", 0);
    optparser.addOption("vbar", 0);
    optparser.addOption("filter", 1, "0");
    optparser.addOption("fc", 1, "0xff00c800");
    optparser.addOption("nanim", 1, "7");
    optparser.addOption("nofont", 0);
    optparser.parse(argc, argv);
    
    if(optparser.isset("help")){
	cout << "Usage: wbar [option] ... [option]" << endl;
	cout << "Options:" << endl;
	cout << "   -help             this help" << endl;
	cout << "   -config conf-file (eg: $HOME/.wbar)" << endl;
	cout << "   -above-desk       run over a desktop app (ie: xfdesktop)" << endl;
	cout << "   -isize  i         icon size (eg: 32)" << endl;
	cout << "   -idist  d         icon dist (eg: 1)" << endl;
	cout << "   -zoomf  z         zoom factor (eg: 1.8 or 2.5)" << endl;
	cout << "   -jumpf  j         jump factor (eg: 1.0 or 0.0)" << endl;
	cout << "   -pos    p         position:" << endl;
	cout << "                        top | bottom | left | right | " << endl;
	cout << "                        center | <bot|top>-<right|left>" << endl;
	cout << "   -dblclk ms        ms for double click (0: single click)" << endl;
	cout << "   -bpress           icon gets pressed" << endl;
	cout << "   -vbar             vertical bar" << endl;
	cout << "   -balfa  i         bar alfa (0-100)" << endl;
	cout << "   -falfa  i         unfocused bar alfa (0-100)" << endl;
	cout << "   -filter i         color filter (0: none 1: hovered 2: others, 3: all)" << endl;
	cout << "   -fc  0xAARRGGBB   filter color (default green 0xff00c800)" << endl;
	cout << "   -nanim  i         number of animated icons: 1, 3, 5, 7, 9, ..." << endl;
	cout << "   -nofont           if set disables font rendering" << endl;
	return 0;
    }
/*}}}*/

    /* window configuration *//*{{{*/
    barwin.setName("wbar");
    if( optparser.isset("above-desk") ){
	barwin.setDesktopWindow();
	barwin.skipTaskNPager();
	barwin.noDecorations();
	barwin.setSticky();
	barwin.bottomLayer();
    }else{
	barwin.setOverrideRedirection();
	barwin.lowerWindow();
    }

    /* tell X what events we're intrested in */
    barwin.selectInput(PointerMotionMask | ExposureMask | ButtonPressMask /* |PropertyChangeMask */
	    | ButtonReleaseMask | LeaveWindowMask | EnterWindowMask);
    /*}}}*/

    /* Image library set up */
    INIT_IMLIB(barwin.getDisplay(), barwin.getVisual(), 
	barwin.getColormap(), barwin.getDrawable(), 2048*2048);

    /* Register handler for recovering corps *//*{{{*/
    sigh.sa_handler = corpshandler;
    sigh.sa_flags = 0;
    sigemptyset(&sigh.sa_mask); //exclude all signals
    sigaction(SIGCHLD, &sigh, NULL);
    /*}}}*/

    /* check if double clicking, ms time */
    dblclk_tm = optparser.isset("dblclk")?atoi(optparser.getArg("dblclk").c_str()):0;
    butpress = optparser.isset("bpress")?1:0;
    vertbar = optparser.isset("vbar")?1:0;

    IconLoader icload(optparser.getArg("config").c_str());
    icload.openConf();

    if(!icload.nextIconInfo(image, command, text)); // should kill the app
    
    /* Create the Bar *//*{{{*/
    if(optparser.isset("balfa") || optparser.isset("falfa") || 
	optparser.isset("filter") || optparser.isset("fr")|| optparser.isset("fa") ||
	optparser.isset("fg")|| optparser.isset("fb") || !(text.empty() || optparser.isset("nofont")) ){
	
	barra = new SuperBar(&barwin, image, text,
	    atoi(optparser.getArg("isize").c_str()),
	    atoi(optparser.getArg("idist").c_str()),
	    atof(optparser.getArg("zoomf").c_str()),
	    atof(optparser.getArg("jumpf").c_str()), 
	    vertbar, 1, atoi(optparser.getArg("nanim").c_str()),
	    atoi(optparser.getArg("balfa").c_str()),
	    atoi(optparser.getArg("falfa").c_str()),
	    atoi(optparser.getArg("filter").c_str()),
	    strtoul(optparser.getArg("fc").c_str(), NULL, 16),
	    optparser.isset("nofont")?0:1);

	/* Load Icon Info */
	while( !icload.nextIconInfo(image, command, text) )
	    ((SuperBar*)barra)->addIcon(image, command, text);

    }else{
	barra = new Bar(&barwin, image,
	    atoi(optparser.getArg("isize").c_str()),
	    atoi(optparser.getArg("idist").c_str()),
	    atof(optparser.getArg("zoomf").c_str()),
	    atof(optparser.getArg("jumpf").c_str()), 
	    vertbar, 1, atoi(optparser.getArg("nanim").c_str()));

	/* Load Icon Info */
	while( !icload.nextIconInfo(image, command, text) )
	    barra->addIcon(image, command);
    }
/*}}}*/

    icload.closeConf();

    /* Show the Bar *//*{{{*/

    if( optparser.isset("above-desk") ){
	barwin.mapWindow();
	barra->setPosition(optparser.getArg("pos"));
    }else{
	barra->setPosition(optparser.getArg("pos"));
	barwin.mapWindow();
    }

    barra->refresh();
    /*}}}*/

    /* Event Loop *//*{{{*/
    while( true ){

	barwin.nextEvent(&ev);
	switch(ev.type){

	    case Expose:
		barra->refresh();
		break;

	    /* Button Press *//*{{{*/
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
		    case 4: //wheel up
			//barra->setZoom(barra->getZoom()+0.1);
			//barra->scaleIcons(ev.xbutton.x);
			break;
		    case 5:
			//barra->setZoom(barra->getZoom()-0.1);
			//barra->scaleIcons(ev.xbutton.x);
			break;
		}
		break;
	    /*}}}*/

	    /* Button Release *//*{{{*/
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

	    /* Motion Notify *//*{{{*/
	    case MotionNotify:
		if(!vertbar)
		    barra->refresh(ev.xmotion.x);
		else
		    barra->refresh(ev.xmotion.y);
		break;
	    /*}}}*/

	    /* Leave & Enter Notify *//*{{{*/
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
	    /*}}}*/

	    default:
		break;
		
	}
    }
    /*}}}*/

    delete barra;
    return 0;

}catch(const char *m){
    cout << m << endl;
}

void corpshandler(int sig){
    while(waitpid(-1 ,NULL, WNOHANG)>0); 
}
