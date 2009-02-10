#include <math.h>
#include <sys/time.h>
#include <string>

#include "Bar.h"
#include "Image.h"

// Posicionamiento de la ventana
#define MARGEN 4

Bar::Bar(XWin *win, const char *bpath) :
    // Initialization
    buffer(1, 1), bar(bpath), window(win), icon_dist(1), 
    icon_size(32), icon_anim(7), jump_factor(1.0), zoom_factor(1.8),
    zoomed_icon(-1), focused(0) {

}

Bar::~Bar() {
    while( !icons.empty() ) {
        delete icons.back();
        icons.pop_back();
    }
}

void Bar::addIcon(std::string path, std::string comm) {

    icons.push_back( new Icon(path.c_str(), comm.c_str()) );
}

void Bar::scale() {
    float up_growth, dn_growth, b_scl_c;

    // unit icon
    icon_unit = (icon_size + icon_dist);
    // icons on side
    icon_ansd = (icon_anim-1)/2;
    // icon offset constant
    icon_offset =  icon_ansd * ( (zoom_factor>1)?(zoom_factor-1) : 0.5 ) * icon_unit;
    // Icon scaling constant
    b_dd = (zoom_factor>1)? (zoom_factor-1)*icon_unit : zoom_factor*icon_unit;

    // x0 = (a + (icon_anim + 1)/2) * icon_unit;
    // x1 = (a - (icon_anim - 1)/2) * icon_unit;
    // xm = (a + 0.5) * icon_unit;

#ifdef COS_ZOOM
    b_scl_b = 3.14159 / icon_unit / icon_anim;
#else
    // Estos calculos de escala son en base a la funcion sigmoidea f = 1/(1+exp(a+bx^2))
    b_scl_c = 0.01; // mientras mas cercano a 0 mas se desplaza chico
    b_scl_d = 0.20; // mientras mas cercano a 1 mas se desplaza grande -> si quiero pico lo achico
    b_scl_a = logf((1.0-b_scl_d)/b_scl_d);
    //b_scl_b = (logf((1.0-b_scl_c)/b_scl_c) - b_scl_a)/(x0-xm)^2;
    b_scl_b = 4*(logf((1.0-b_scl_c)/b_scl_c) - b_scl_a)/
	(icon_anim*icon_anim*icon_unit*icon_unit);
#endif
#ifdef LINEAR_TRASL
    //float m = (icon_anim-1)*b_dd / (x1 - x0);
    b_pos_m = -icon_ansd * 2 * b_dd / icon_unit / icon_anim;
#else
    b_pos_m = - 3.14159 / icon_unit / icon_anim;
#endif

    // bar dimensions
    width = icon_unit * (icons.size() + 1) - 2 * icon_dist;
    height=(int)(icon_size * 1.25);

    // Space needed for growing
    up_growth = jump_factor*icon_size*(zoom_factor-1);
    dn_growth = icon_size*(zoom_factor-1) - up_growth;

    // substract growth superposed on bar
    up_growth-= icon_size*0.125;
    dn_growth-= icon_size*0.125;

    // Set new window width
    window->w = (int)(width + 2*icon_offset);
    window->h = height + 2*MARGEN +	
        (int)((up_growth>0.0? up_growth : 0.0))+
        (int)((dn_growth>0.0? dn_growth : 0.0));

    // Initial Bar position
    y = MARGEN + (int)(up_growth>0.0? up_growth : 0.0);
    x = (int)icon_offset;

    // Re organize widget position
    for(size_t a=0; a < icons.size(); a++) {
        icons[a]->x = icons[a]->ox = (int)(icon_offset + icon_size/2 + a * icon_unit);
        icons[a]->y = icons[a]->oy = y + (int)(0.125 * icon_size);
        icons[a]->size = icon_size;
    }

    // Adapt container window to changes
    window->move_resize(window->x, window->y, window->w, window->h);
    window->go_transparent();
    buffer = Image(window->w, window->h);
}

void Bar::transform(int mousex) { 
#ifdef LINEAR_TRASL
    int x0 = (icon_ansd+1) * icon_unit; // last movable pos
#endif

    int xx = mousex + (int)(icon_offset + icon_size/2);
    int	rx = mousex - icon_unit/2;
    int i_num = mousex / icon_unit;
    int t_x, t_y, a;
    Icon *cur_ic=0;

    zoomed_icon = -1 ; 

#ifdef LINEAR_TRASL
    for(a=0; a<(int)icons.size(); a++, rx -= icon_unit, x0 += icon_unit ){
#else
    for(a=0; a<(int)icons.size(); a++, rx -= icon_unit ){
#endif

        cur_ic = icons[a];
        
        cur_ic->bx = cur_ic->x;
        cur_ic->by = cur_ic->y;
        cur_ic->bs = cur_ic->size;

        /* Check if over the icon */
        if(zoomed_icon == -1)
            if(xx >= cur_ic->x && xx < cur_ic->x + cur_ic->size)
		        zoomed_icon = a;

        if( ((a<i_num)?i_num-a:a-i_num) > icon_ansd ){

            t_x = (int)(cur_ic->ox + ((a<i_num)?-1:1) * icon_ansd * b_dd );
            t_y = cur_ic->oy;

            if(cur_ic->size != icon_size || cur_ic->x != t_x || cur_ic->y != t_y){

                cur_ic->size = icon_size;
                cur_ic->x = t_x;
                cur_ic->y = t_y;
            }
        
        } else {

#ifdef COS_ZOOM
            cur_ic->size = (int)( icon_size * 
                ( 1.0 + (zoom_factor-1.0)*cosf(b_scl_b * rx)));
#else
            cur_ic->size = (int)( icon_size * 
                ( 1.0 + (zoom_factor-1.0)/(1.0 + expf(b_scl_a + b_scl_b*rx*rx))/b_scl_d));
#endif

#ifdef LINEAR_TRASL
            cur_ic->x = cur_ic->ox - (int)(icon_ansd * b_dd - b_pos_m * (mousex - x0));
#else 
            cur_ic->x = cur_ic->ox + (int)(icon_ansd * b_dd * sinf(b_pos_m * rx));
#endif


            cur_ic->y = cur_ic->oy - (int)(jump_factor*(cur_ic->size-icon_size));
	    }
    }
}

void Bar::render(){
    Icon *cur_ic=0;

    //buffer.colorClear(0, 0, 0, 0);
    buffer.full() += window->background.full();
    buffer.subImage(x, y, width, height) |= bar.full();

    // Blend Icons
    for(size_t a=0; a<icons.size(); a++){

        if((int)a == zoomed_icon)
            continue;

        cur_ic = icons[a];

        buffer.subImage(cur_ic->x, cur_ic->y, 
            cur_ic->size, cur_ic->size) |= cur_ic->icon.full();
    }

    if(zoomed_icon != -1) {
        cur_ic = icons[zoomed_icon];
        buffer.subImage(cur_ic->x, cur_ic->y, 
            cur_ic->size, cur_ic->size) |= cur_ic->render();
    }

    buffer.splat(window->window);
}

void Bar::refresh(int mouse_x){

    // coords relative to icon offset
    mouse_x -= (int)(icon_offset + icon_size/2.0);

    // on the bar
    if(mouse_x > 0 && mouse_x < (int)icons.size() * icon_unit) {

        transform(mouse_x);

        if( !focused )
            set_focus( 1 );

    } else // out of the bar
        if(focused)
            set_focus( 0 );

    render();
}

void Bar::animate() {
    Icon *cur_ic=0;

    /* animation time in ms */
    int t=0, anim_time = 80;
    struct timeval tv0, tv;

    for(size_t i = 0; i < icons.size(); i++){
        cur_ic = icons[i];
        cur_ic->vx = (cur_ic->x - cur_ic->bx) / (float)anim_time;
        cur_ic->vy = (cur_ic->y - cur_ic->by) / (float)anim_time;
        cur_ic->vs = (cur_ic->size - cur_ic->bs) / (float)anim_time;
    }

    gettimeofday(&tv0, NULL);

    while(t < anim_time){
        gettimeofday(&tv, NULL);

        t = (int)((tv.tv_sec - tv0.tv_sec) * 1000 + (tv.tv_usec - tv0.tv_usec)/1000);
        /* Printing t here can tell you the max system responsiveness */
        if(t > anim_time) t = anim_time;

        for(size_t j = 0; j< icons.size(); j++){
            cur_ic = icons[j];

            cur_ic->x = cur_ic->bx + (int)(t * cur_ic->vx);
            cur_ic->y = cur_ic->by + (int)(t * cur_ic->vy);
            cur_ic->size = cur_ic->bs + (int)(t * cur_ic->vs);
        }

        render();
    }
}

void Bar::set_focus(int focus) {
    if( focus ) {
        focused = 1;
        x=0;
        width = window->w;
    } else {
        focused = 0;
        x = (int)icon_offset;
        width = (int)(window->w - 2*icon_offset);
        zoomed_icon = -1;

        for(size_t a=0; a<icons.size(); a++) {
            icons[a]->x = icons[a]->ox;
            icons[a]->y = icons[a]->oy;
            icons[a]->size = icon_size;
        }
    }

    animate();
}
