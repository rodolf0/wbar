#ifndef _BAR_H_
#define _BAR_H_

#include <vector>
#include <string>
#include "Image.h"
#include "XWin.h"
#include "Icon.h"

class OptParser;

class Bar{

    public:

        Bar(XWin *win, const char *barImg);
        ~Bar();

        /* add an Icon */
        void addIcon(std::string path, std::string comm);

        /* Focus & unfocus events */
        void refresh(int mouse_x);

        /* set bar */
        void set_focus(int focus);

        int icon_index(int mouse_x) const;
        std::string icon_cmd(int idx);

    protected:

        friend class OptParser;

        // Drawing buffs
        Image cleanbuf;
        Image buffer;
        Image bar;
        // reference to the container window
        XWin *window;
        std::vector<Icon*> icons;

        // Bar config parameters
        int icon_dist;
        int icon_size;
        int icon_anim;
        float jump_factor;
        float zoom_factor;

        // fast math constants
        float b_scl_d;
        float b_scl_a;
        float b_scl_b;
        float b_pos_m;
        float b_pos_n;
        float b_dd;
        float icon_offset;
        int icon_unit;
        int icon_ansd;

        // current status
        int x, y, width, height;
        int zoomed_icon;
        int focused;

        void transform(int mousex);
        void render();
        void animate();
        void scale();
};

#endif /* _BAR_H_ */
