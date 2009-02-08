#ifndef _ICON_H_
#define _ICON_H_

#include <string>
#include "Image.h"

class Bar;

class Icon{
    public:

        Icon(const char *iconImg, const char *cmd, int x, int y);
        ~Icon();

    protected:
        friend class Bar;

        ImlibImage icon;

        // original state
        int ox, oy;
        int osize;
        // current state
        int x, y;
        int size;
        // animation
        int bx, by, bs;
        float vx, vy, vs;

        std::string command;
};

#endif /* _ICON_H_ */
