#include "Icon.h"

using namespace std;

Icon::Icon(string iconImg, string cmd, int xx, int yy) :
    ox(xx), oy(yy), x(xx), y(yy), need_update(1), cx(xx), command(cmd){

    if( !(icon = LOAD_IMAGE(iconImg.c_str())) )
	throw (iconImg + " -> Couldn't load icon image.").c_str();

    USE_IMAGE(icon);
    osize = size = IMAGE_WIDTH();
}    

Icon::~Icon(){
    FREE_IMAGE(icon);
}
