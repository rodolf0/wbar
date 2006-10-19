#include "Bar.h"

/* Posicionamiento de la ventana */
#define MARGEN 8

using namespace std;

/* Bar Constructor & Destructor *//*{{{*/
Bar::Bar(XWin *win, string barImg, int iSize, int iDist, float zFactor, 
	float jFactor, int bOrient, int bPosition) :

    /* Initialization */
    buffer(0), barback(0), bar(0), window(win), icon_dist(iDist),
    icon_size(iSize), jump_factor(jFactor), zoom_factor(zFactor),
    orientation(bOrient), position(bPosition), zoomed_icon(-1), focused(1){

    /* Load Bar back ground */
    if( !(bar = LOAD_IMAGE(barImg.c_str())) )
	throw (barImg + " -> Image not found. Maybe using a relative path?").c_str();

    /* Get Bar Dimensions */
    USE_IMAGE(bar);

    owidth = IMAGE_WIDTH(); oheight = IMAGE_HEIGHT();
    if( orientation == 1 ) IMAGE_TRANSPOSE();
       
    /* Enable Bar's alpha channel */
    IMAGE_ENABLE_ALPHA(1);

    /* set bar initial values */
    scale();
}

Bar::~Bar(){
    FREE_IMAGE(buffer);
    FREE_IMAGE(barback);
    FREE_IMAGE(bar);

    while( !icons.empty() ){
	delete icons.back();
	icons.pop_back();
    }
}
/*}}}*/

/* Add Icon *//*{{{*/
void Bar::addIcon(string path, string comm){

    icons.push_back( new Icon(path, comm,
	x + icon_size/2 + icons.size() * (icon_size + icon_dist), // x coord
	y + (int)(0.125 * icon_size)) ); // y coord

    /* not efficient to call it here but ... user doesn't have to call it */
    scale();
}
/*}}}*/

/* Scale bar *//*{{{*/
void Bar::scale(bool updateBG){
    float up_growth, dn_growth;

    /* unit icon */
    icon_unit = (icon_size + icon_dist);
    /* icon offset constant */
    icon_offset = ( (zoom_factor>1)? (zoom_factor-1) : 0.5 ) * icon_unit;
    /* Icon scaling constant */
    b_dd = (zoom_factor>1)? (2*(zoom_factor-1)*icon_unit) : zoom_factor*icon_unit;


    /* bar dimensions */
    width = icon_unit * (icons.size() + 1) - 2 * icon_dist;
    height=(int)(icon_size * 1.25);

    /* Space needed for growing */
    up_growth = jump_factor*icon_size*(zoom_factor-1);
    dn_growth = icon_size*(zoom_factor-1) - up_growth;

    /* substract growth superposed on bar */
    up_growth-= icon_size*0.125;
    dn_growth-= icon_size*0.125;

    /* Set new window width */ 
    window->w = (int)(width + 2*icon_offset);
    window->h = height + 2*MARGEN +	
	(int)((up_growth>0.0? up_growth : 0.0))+
	(int)((dn_growth>0.0? dn_growth : 0.0));

    /* Initial Bar position */
    y = MARGEN + (int)(up_growth>0.0? up_growth : 0.0);
    x = (int)icon_offset;

    if(updateBG) 
	acquireBack();
}
/*}}}*/

/* Acquire background *//*{{{*/
void Bar::acquireBack(){
    int t_w, t_h;

    t_w = (orientation == 0)? window->w : window->h;
    t_h = (orientation == 0)? window->h : window->w;

    /* Update win x && y for getting root img *//*{{{*/
    switch(position){
	case 1:	case 2:	case 5:
	    window->x = (window->screenWidth() - t_w)/2;
	    break;

	case 3:	case 7:	case 9:
	    window->x = 0;
	    break;

	case 4:	case 6:	case 8:
	    window->x = window->screenWidth() - t_w;
	    break;
    }

    switch(position){
	case 3:	case 4:	case 5:
	    window->y = (window->screenHeight() - t_h)/2;
	    break;

	case 1:	case 6:	case 7:
	    window->y = window->screenHeight() - t_h;
	    break;

	case 2:	case 8:	case 9:
	    window->y = 0;
	    break;
    }
    /*}}}*/

    /* Move & resize win for fitting new icons */
    window->moveNresize(window->x, window->y, t_w, t_h);

    if(barback) FREE_IMAGE(barback);
    if(buffer) FREE_IMAGE(buffer);

    buffer = CREATE_IMAGE(t_w, t_h);

    /* Get background Image */
    USE_DRAWABLE(DefaultRootWindow(window->display));
    barback = IMAGE_FROM_DRAWABLE(window->x, window->y, t_w, t_h);
    USE_DRAWABLE(window->window);
}
/*}}}*/

/* Icons Scaling *//*{{{*/
void Bar::transform(int i_num, int i_off){ 
    int t_x, t_y, a;

    Icon *cur_ic=0;

    /* the Big icon *//*{{{*/
    /* Working with the Big Icon */
    cur_ic = icons[i_num];

    /* check wether we're over the icon *//*{{{*/
    /* i_off > i_x-i_ox  &&
     * i_off < i_x+i_s-i_ox of zoomed icon? => on it */
    if(i_off > b_dd/(4+2*b_dd/icon_unit) &&
       i_off < (b_dd+4*cur_ic->size)/(4+2*b_dd/icon_unit))
	zoomed_icon = i_num;
    else
	zoomed_icon = -1 ; 
    /*}}}*/

    /* interpolate zoom increment
     * 0-> (z-1)/2; 
     * maxoff/2-> (z-1); 
     * maxoff-> (z-1)/2 */
    cur_ic->cx = cur_ic->x;
    cur_ic->csize = cur_ic->size;

    cur_ic->size =(int)( icon_size 
	+((zoom_factor-1)/2
	+(zoom_factor-1)/icon_unit*i_off
	-2*(zoom_factor-1)/icon_unit/icon_unit*
	    i_off*(i_off-icon_unit/2))*icon_size
	);
    
    /* max offset => x = xo-dd/2+dd/4
     * min offset => x = xo-dd/2+3*dd/4 */
    cur_ic->x =(int)( cur_ic->ox + b_dd/4
	- b_dd/2/icon_unit*i_off );

    /* keep all icons on same floor subtracting what it grew */
    cur_ic->y =  cur_ic->oy-
	(int)(jump_factor*(cur_ic->size-icon_size)) ;

    /* Update this icon, important uh?*/
    cur_ic->need_update = 1;
    /*}}}*/

    /* Working with zoom - 1 Icon *//*{{{*/
    if(i_num>0){
	cur_ic = icons[i_num-1];

	/* Used for restoring background */
	cur_ic->cx = cur_ic->x;
	cur_ic->csize = cur_ic->size;

	/* Icon scl factor 1 at max offset, 1.5 at min offs */
	cur_ic->size = (int)( icon_size
	    + ((zoom_factor-1)/2 
	    - (zoom_factor-1)/icon_unit/2*i_off) * icon_size );

    /* max offset => x = xo-dd/2
     * min offset => x = xo-dd/2+dd/4 */
	cur_ic->x = (int)( cur_ic->ox - b_dd/4
	    - b_dd/4/icon_unit*i_off );

	/* Icons on the same floor */
	cur_ic->y = cur_ic->oy-
	    (int)(jump_factor*(cur_ic->size-icon_size));
	/* Update new size n pos */
	cur_ic->need_update = 1;
    }
    /*}}}*/

    /* Working with zoom + 1 Icon *//*{{{*/
    if(i_num < (int)icons.size()-1){
	cur_ic = icons[i_num+1];
    
	cur_ic->cx = cur_ic->x;
	cur_ic->csize = cur_ic->size;

	/* Icon scl factor 1 at min offset, 1.5 at max offs */
	cur_ic->size = (int)( icon_size
	    + ((zoom_factor-1)/icon_unit/2*i_off) * icon_size );
		
	/* max offset => x = xo-dd/2+3*dd/4
	 * min offset => x = xo-dd/2+dd */
	cur_ic->x = (int)( cur_ic->ox + b_dd/2
	    - b_dd/4/icon_unit*i_off );

	/* Icons on the same floor */
	cur_ic->y = cur_ic->oy-
	    (int)(jump_factor*(cur_ic->size-icon_size));
	cur_ic->need_update = 1;
    }
    /*}}}*/

    /* previous icons *//*{{{*/
    for(a=0; a<i_num-1; a++){
	cur_ic = icons[a];

	t_x = (int)( cur_ic->ox - b_dd/2 );
	t_y = cur_ic->oy;
	if(cur_ic->size != icon_size ||
		cur_ic->x != t_x ||
		cur_ic->y != t_y){
    
	    cur_ic->cx = cur_ic->x;
	    cur_ic->csize = cur_ic->size;

	    cur_ic->size = icon_size;
	    cur_ic->x = t_x;
	    cur_ic->y = t_y;
	    cur_ic->need_update = 1;
	}//else cur_ic->need_update = 0;
    }
    /*}}}*/

    /* next icons *//*{{{*/
    for(a=i_num+2; a<(int)icons.size(); a++){
	cur_ic = icons[a];
	t_x = (int)( cur_ic->ox + b_dd/2 );
	t_y = cur_ic->oy;
	if(cur_ic->size != icon_size ||
		cur_ic->x != t_x ||
		cur_ic->y != t_y){
    
	    cur_ic->cx = cur_ic->x;
	    cur_ic->csize = cur_ic->size;

	    cur_ic->size = icon_size;
	    cur_ic->x = t_x;
	    cur_ic->y = t_y;
	    cur_ic->need_update = 1;
	}//else cur_ic->need_update = 0;
    }
    /*}}}*/

}
/*}}}*/

/* Clean the Bar *//*{{{*/
void Bar::cleanBack(){
    Icon *cur_ic;

    USE_IMAGE(buffer);

    for(size_t a=0; a<icons.size(); a++){
	cur_ic = icons[a];

	if(cur_ic->need_update == 0) continue;

	/* Copy Root Background */
	SET_BLEND(0);

	if(orientation == 0){
	    BLEND_IMAGE(barback, cur_ic->cx, 0, cur_ic->csize, window->h, 
		    cur_ic->cx, 0, cur_ic->csize, window->h);
	    
	    /* Blend the bar */
	    SET_BLEND(1);
	    BLEND_IMAGE(bar, cur_ic->cx * owidth / width, 0, 
		    cur_ic->csize * owidth / width, oheight,
		    cur_ic->cx, y, cur_ic->csize, height);

	}else{
	    BLEND_IMAGE(barback, 0, cur_ic->cx, window->h, cur_ic->csize, 
		    0, cur_ic->cx, window->h, cur_ic->csize);

	    /* Blend the bar */
	    SET_BLEND(1);
	    BLEND_IMAGE(bar, 0, cur_ic->cx * owidth / width,
		    oheight, cur_ic->csize * owidth / width,
		    y, cur_ic->cx, height, cur_ic->csize);
	}
    }
}

void Bar::drawBack(){
    
    USE_IMAGE(buffer);

    /* Copy Root Background */
    SET_BLEND(0);

    if(orientation == 0){
	BLEND_IMAGE(barback, 0, 0, window->w, window->h, 
		0, 0, window->w, window->h);
	
	/* Blend the bar */
	SET_BLEND(1);
	BLEND_IMAGE(bar, 0, 0, owidth, oheight, x, y, width, height);

    }else{
	BLEND_IMAGE(barback, 0, 0, window->h, window->w, 
		0, 0, window->h, window->w);

	/* Blend the bar */
	SET_BLEND(1);
	BLEND_IMAGE(bar, 0, 0, oheight, owidth, y, x, height, width);
    }

}
/*}}}*/

/* Render the Bar *//*{{{*/
void Bar::render(){
    Icon *cur_ic;

#ifdef COOLMACHINE_N_SHITCODE
    drawBack();
#endif

    /* Set work area */
    USE_IMAGE(buffer);
    SET_BLEND(1);

    /* Blend the zoomed icon last */
    if(zoomed_icon != -1){
	cur_ic = icons.back();
	icons.back() = icons[zoomed_icon];
	icons[zoomed_icon] = cur_ic;
    }

    /* Blend Icons */
    for(size_t a=0; a<icons.size(); a++){
	cur_ic = icons[a];
	
	/* If Icon needs update => blend it */
#ifdef COOLMACHINE_N_SHITCODE
	if(cur_ic->need_update != 9){
#else
	if(cur_ic->need_update == 1){
#endif

	    cur_ic->need_update = 0;

	    if(orientation == 0)
		BLEND_IMAGE(cur_ic->icon, 0, 0, cur_ic->osize, cur_ic->osize, 
		    cur_ic->x, cur_ic->y, cur_ic->size, cur_ic->size);
	    else
		BLEND_IMAGE(cur_ic->icon, 0, 0, cur_ic->osize, cur_ic->osize, 
		    cur_ic->y, cur_ic->x, cur_ic->size, cur_ic->size);
	}
    }

    if(zoomed_icon != -1){
	cur_ic = icons.back();
	icons.back() = icons[zoomed_icon];
	icons[zoomed_icon] = cur_ic;
    }
    
    /* Show the buffer */
    SET_BLEND(0);
    RENDER_TO_DRAWABLE(0, 0);

}
/*}}}*/

/* Refresh *//*{{{*/
void Bar::refresh(int mouse_x){
    Icon *cur_ic;

    int iNum = iconNumber(mouse_x);
    int iOff = iconOffset(mouse_x);

    /* on the bar */
    if(mouse_x > (int)(icon_offset + icon_size/2.0) && iNum < (int)icons.size()){

	if(!focused) focus();

	transform(iNum, iOff);
#ifndef COOLMACHINE_N_SHITCODE
	cleanBack();
#endif

    /* out of the bar */
    }else{

	if(focused) unfocus();

	for(size_t a=0; a < icons.size(); a++){

	    cur_ic = icons[a];

	    if(cur_ic->size == icon_size && cur_ic->x == cur_ic->ox &&
		cur_ic->y == cur_ic->oy) continue;

	    cur_ic->cx = cur_ic->x;
	    cur_ic->csize = cur_ic->size;

	    cur_ic->x = cur_ic->ox;
	    cur_ic->y = cur_ic->oy;

	    cur_ic->size = icon_size;
	    cur_ic->need_update = 1;
	}
    }

    render();
}
/*}}}*/

/* In & out of the bar *//*{{{*/
inline void Bar::unfocus(){
    focused = 0;

    x = (int)icon_offset;
    width = (int)(window->w - 2*icon_offset);
    zoomed_icon = -1;

    drawBack();
}

inline void Bar::focus(){
    focused = 1;

    x=0;
    width = window->w;

    drawBack();
}
/*}}}*/

/* Icon number && offset */
int Bar::iconNumber(int mouse_x){
    return (mouse_x - (int)(icon_offset + icon_size/2.0)) / icon_unit;
}
int Bar::iconOffset(int mouse_x){
    return (mouse_x - (int)(icon_offset + icon_size/2.0)) % icon_unit;
}

/* Icon press / release events *//*{{{*/
inline void Bar::iconPress(int i_num, int offs){
    Icon *cur_ic;

    if(i_num<0 || i_num>=(int)icons.size()) return;

    cur_ic = icons[i_num];

    //cleanI(cur_ic->x, cur_ic->size);
    cur_ic->cx = cur_ic->x;
    cur_ic->csize = cur_ic->size;

    cur_ic->x += offs;
    cur_ic->y += offs;

    cur_ic->need_update = 1;

    /* this should fix nex icon blanking */
    if(i_num < (int)icons.size()-1)
	icons[i_num+1]->need_update = 1;

    cleanBack();
    render();
}

void Bar::iconUp(int i_num){
    iconPress(i_num, -2);
}

void Bar::iconDown(int i_num){
    iconPress(i_num, 2);
}
/*}}}*/

/* Position the Bar *//*{{{*/
void Bar::setPosition(string pos){
    
    if( !pos.compare("bottom") ) position = 1;
    else if( !pos.compare("top") ) position = 2;
    else if( !pos.compare("left") ) position = 3;
    else if( !pos.compare("right") ) position = 4;
    else if( !pos.compare("center") ) position = 5;
    else if( !pos.compare("bot-right") ) position = 6;
    else if( !pos.compare("bot-left") ) position = 7;
    else if( !pos.compare("top-right") ) position = 8;
    else if( !pos.compare("top-left") ) position = 9;
    else throw (pos + " -> Invalid bar position.").c_str();

    acquireBack();
}
/*}}}*/

/* Icon info *//*{{{*/
string Bar::iconCommand(int i_num){

    if(i_num<0 || i_num>=(int)icons.size()) return NULL;
    
    return icons[i_num]->command;
}

int Bar::iconIndex(int mouse_x){
    int i_num = iconNumber(mouse_x);
    int i_off = iconOffset(mouse_x);

    if(mouse_x > (int)(icon_offset + icon_size/2.0) && i_num < (int)icons.size()){
	if(i_off > b_dd/(4+2*b_dd/icon_unit) &&
	   i_off < (b_dd+4*icons[i_num]->size)/(4+2*b_dd/icon_unit))
	    return i_num;
    }
    return -1;
}
/*}}}*/

#if 0
/* Zoom beans *//*{{{*/
void Bar::setZoom(float zoomf){
    b_zoom_f = zoomf;
    scale(false);
}

float Bar::getZoom(){
    return b_zoom_f;
}
/*}}}*/
#endif

