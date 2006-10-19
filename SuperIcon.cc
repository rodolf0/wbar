#include <string.h>
#include "SuperIcon.h"

using namespace std;

SuperIcon::SuperIcon(string iconImg, string cmd, string txt, 
	int x, int y, int tw, int th) : 
    Icon(iconImg, cmd, x, y), textW(tw), textH(th), text(txt){

    USE_IMAGE(icon);
    icon_color = CLONE_IMAGE();
}

SuperIcon::~SuperIcon(){
    FREE_IMAGE(icon_color);
}
