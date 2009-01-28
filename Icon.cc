#include "Icon.h"

Icon::Icon(std::string iconImg, std::string cmd, int xx, int yy) :
    icon( 1, 1 ), ox(xx), oy(yy), x(xx), y(yy), need_update(1), cx(xx), command(cmd) {

    icon = ImlibImage(iconImg.c_str());

    osize = size = icon.ow;
}    
