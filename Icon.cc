#include "Icon.h"

Icon::Icon(const char *iconImg, const char *cmd, int xx, int yy) :
    icon( 1, 1 ), ox(xx), oy(yy), x(xx), y(yy), command(cmd) {

    icon = ImlibImage( iconImg );

    osize = size = icon.ow;
}    

Icon::~Icon() {}
