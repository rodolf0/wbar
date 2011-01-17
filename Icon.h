#ifndef _ICON_H_
#define _ICON_H_

#include <string>
#include "Image.h"

class Bar;

class Icon{
  public:

    Icon(const char *iconImg, const char *cmd);
    ~Icon();

    Image& render();

  protected:
    friend class Bar;

    Image icon;

    // original state
    int ox, oy;
    // current state
    int x, y;
    int size;
    // animation
    int bx, by, bs;
    int cx, cy, cs;
    float vx, vy, vs;

    std::string command;
};

#endif /* _ICON_H_ */
