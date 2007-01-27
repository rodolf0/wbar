#include "SuperBar.h"
#include "SuperIcon.h"

using namespace std;

/* Super Constructor *//*{{{*/
SuperBar::SuperBar(XWin *win, string barImg, string barFont, int iSize, int iDist, 
    float zFactor, float jFactor, int bOrient, int bPosition, int nAnim, int barAlfa, 
    int unfocusAlfa, int filtSel, unsigned int filtCol, bool dfont) :

    Bar(win, barImg, iSize, iDist, zFactor, jFactor, bOrient, bPosition, nAnim), 
    font(NULL), font_w(0), drawfont(dfont), filtSel(filtSel), filtRed((filtCol & 0x00ff0000)>>16), 
    filtGreen((filtCol & 0x0000ff00)>>8), filtBlue(filtCol & 0x000000ff),
    filtAlfa((filtCol & 0xff000000)>>24), unfocusAlfa(unfocusAlfa), barAlfa(barAlfa) { 

    int textW, textH;

    initFilters();

    /* Text Printing */
    if( !barFont.empty() ){
	ADD_FONT_PATH(getenv("HOME"));
	if( !(font = LOAD_FONT(barFont.c_str())) )
	    throw (barFont + " -> Couldn't load font.").c_str();
	USE_FONT(font);

	imlib_get_text_size("MMMMMMMMMMMMMMM", &textW, &textH);
	font_restore = imlib_create_image(textW, 2*textH);
    }
}

SuperBar::~SuperBar(){

    if( font )
	FREE_IMAGE(font_restore);

    if(filtSel != 0){
	imlib_context_set_filter(colorFilter);
	imlib_free_filter();
    }

    if(unfocusAlfa != -1){
        imlib_context_set_filter(fAlfaUnfocus);
	imlib_free_filter();
    }

    if(font){
	imlib_context_set_font(font);
	imlib_free_font();
    }
}
/*}}}*/

/* Filter Images: *//*{{{*/
void SuperBar::initFilters(){
    Imlib_Filter fAlfaBar;

    /* Color filters */
    if(filtSel != 0){
	colorFilter = imlib_create_filter(0);
	imlib_context_set_filter(colorFilter);

#ifndef AVGFILTER
	/* Idem a lo que esta abajo
	imlib_filter_set_red  (0, 0, 0, filtRed, 0, 0);
	imlib_filter_set_green(0, 0, 0, 0, filtGreen, 0);
	imlib_filter_set_blue (0, 0, 0, 0, 0, filtBlue);
	imlib_filter_set_alpha(0, 0, filtAlfa, 0, 0, 0); // new
	*/
	imlib_filter_set(0, 0, filtAlfa, filtRed, filtGreen, filtBlue);
	imlib_filter_divisors(255, 255, 255, 255);
#else
	/* 100 en vez de 1 para no usar comma */
	/* explicacion: (r+g+b)/3 = gray
	 *
	 * (r+g+b)/(3*R) = tonos de rojo ( R -> [0 - 1])
	 * (r+g+b)/(3*G) = tonos de verd ( G -> [0 - 1])
	 * (r+g+b)/(3*B) = tonos de blue ( B -> [0 - 1])
	 * */

	if(filtRed != 0)
	    imlib_filter_set_red  (0, 0, 0, 1, 1, 1);
	else
	    imlib_filter_set_red  (0, 0, 0, 0, 0, 0);

	if(filtGreen != 0)
	    imlib_filter_set_green(0, 0, 0, 1, 1, 1);
	else
	    imlib_filter_set_green(0, 0, 0, 0, 0, 0);

	if(filtBlue != 0)
	    imlib_filter_set_blue (0, 0, 0, 1, 1, 1);
	else
	    imlib_filter_set_blue (0, 0, 0, 0, 0, 0);

	imlib_filter_set_alpha(0, 0, filtAlfa, 0, 0, 0); // new
	imlib_filter_divisors(255, filtRed!=0?3*255/filtRed:1, 
		filtGreen!=0?3*255/filtGreen:1, filtBlue!=0?3*255/filtBlue:1);
#endif

    }

    /* Set alfa to the Bar */
    if(barAlfa != -1){
	fAlfaBar = imlib_create_filter(0);
	imlib_context_set_filter(fAlfaBar); 
	imlib_filter_set_alpha(0, 0, barAlfa, 0, 0, 0);
	imlib_filter_divisors(100, 0, 0, 0);

	USE_IMAGE(bar);
	imlib_image_filter();
	imlib_free_filter();
    }

    /* Alfa the bar when loose focus */
    if(unfocusAlfa != -1){
	fAlfaUnfocus = imlib_create_filter(0);
	imlib_context_set_filter(fAlfaUnfocus); 
	imlib_filter_set_alpha(0, 0, unfocusAlfa, 0, 0, 0);
	imlib_filter_divisors(100, 0, 0, 0);
    }
}
/*}}}*/

/* Add Icon to the bar *//*{{{*/
void SuperBar::addIcon(string path, string comm, string txt){
    SuperIcon *ic;
    int textW, textH;

    if(font){
	imlib_context_set_font(font);
	imlib_get_text_size(txt.c_str(), &textW, &textH);
    }

    icons.push_back( new SuperIcon(path, comm, txt,
	x + icon_size/2 + icons.size() * (icon_size + icon_dist), // x coord
	y + (int)(0.125 * icon_size), textW, textH) ); // y coord

    ic = (SuperIcon*)icons.back();

    /* Set colored icon */
    if(filtSel != 0){
	USE_IMAGE(ic->icon_color);
	imlib_context_set_filter(colorFilter);
	imlib_image_filter();
    }

    scale();
}
/*}}}*/

/* Unfocus *//*{{{*/
inline void SuperBar::unfocus(){
    _image tBar = NULL;

    focused = 0;

    x = (int)icon_offset;
    width = (int)(window->w - 2*icon_offset);
    zoomed_icon = -1;

    /* Alfa filter the Bar */
    if(unfocusAlfa != -1){
	USE_IMAGE(bar);
	tBar = CLONE_IMAGE();
	imlib_context_set_filter(fAlfaUnfocus);
	imlib_image_filter();
    }

    drawBack();

    /* Restore bar */
    if(unfocusAlfa != -1){
	FREE_IMAGE(bar);
	bar = tBar;
    }
}
/*}}}*/

/* Render the Bar *//*{{{*/
void SuperBar::render(){
    SuperIcon *cur_ic;
    _image cur_im = NULL, tIcon;
    int tw = 0, th = 0;

    /* save text coords and render zoomed ic last *//*{{{*/
    /* restore image under text */
    if(font && drawfont)
	if(font_w != 0 )
	    BLEND_IMAGE(font_restore, 0, 0, font_w, font_h, font_x, font_y, font_w, font_h);

    if(zoomed_icon != -1){
	/* Blend the zoomed icon last */
	cur_ic = (SuperIcon*)icons.back();
	icons.back() = icons[zoomed_icon];
	icons[zoomed_icon] = cur_ic;


	if(font && drawfont){
	    cur_ic = (SuperIcon*)icons.back();
	    /* text drawing cords */
	    if(orientation == 0){
		tw = cur_ic->x - (cur_ic->textW - cur_ic->size)/2;
		th = cur_ic->y + cur_ic->size - cur_ic->textH;
	    }else{
		tw = cur_ic->y - (cur_ic->textW - cur_ic->size)/2;
		th = cur_ic->x + cur_ic->size - cur_ic->textH;
	    }
	
	    /* Keep the image before text */
	    font_x = tw; font_y = th;
	    font_w = cur_ic->textW+2; font_h = cur_ic->textH+2;
	    
	    USE_IMAGE(font_restore);
	    BLEND_IMAGE(buffer, tw, th, font_w, font_h, 0, 0, font_w, font_h);
	}

    }else 
	if(font && drawfont)
	    font_w = 0;
/*}}}*/

    /* Set work area */
    USE_IMAGE(buffer);
    SET_BLEND(1);

    /* Blend Icons */
    for(size_t a=0; a<icons.size(); a++){
	cur_ic = (SuperIcon*)icons[a];
	
	/* If Icon needs update => blend it */
	if(cur_ic->need_update == 1){

	    cur_ic->need_update = 0;

	    /* Select image to blend *//*{{{*/
	    if(a == icons.size()-1 && zoomed_icon != -1){
		switch(filtSel){
		    case 0: case 2:
			cur_im = cur_ic->icon;
			break;
		    case 1: case 3:
			cur_im = cur_ic->icon_color;
			break;
		}

	    }else{
		switch(filtSel){
		    case 0: case 1:
			cur_im = cur_ic->icon;
			break;
		    case 2: case 3:
			cur_im = cur_ic->icon_color;
			break;
		}
	    }
	    /*}}}*/

	    if(!focused && unfocusAlfa != -1){
		USE_IMAGE(cur_im);
		tIcon = CLONE_IMAGE();
		USE_IMAGE(tIcon);
		imlib_context_set_filter(fAlfaUnfocus);
		imlib_image_filter();
		cur_im = tIcon;
		USE_IMAGE(buffer);
	    }

	    if(orientation == 0)
		BLEND_IMAGE(cur_im, 0, 0, cur_ic->osize, cur_ic->osize, 
		    cur_ic->x, cur_ic->y, cur_ic->size, cur_ic->size);
	    else
		BLEND_IMAGE(cur_im, 0, 0, cur_ic->osize, cur_ic->osize, 
		    cur_ic->y, cur_ic->x, cur_ic->size, cur_ic->size);



	    if(font && drawfont){

		if(a == icons.size()-1 && zoomed_icon != -1){

		    imlib_context_set_color(255, 0, 0, 255);
		    imlib_text_draw(tw, th, cur_ic->text.c_str());
		    imlib_context_set_color(255, 255, 255, 255);
		    imlib_text_draw(tw+1, th+1, cur_ic->text.c_str());
		}
	    }

	    if(!focused && unfocusAlfa != -1){
		FREE_IMAGE(cur_im);
		USE_IMAGE(buffer);
	    }
	}
    }

    if(zoomed_icon != -1){
	cur_ic = (SuperIcon*)icons.back();
	icons.back() = icons[zoomed_icon];
	icons[zoomed_icon] = cur_ic;
    }
    
    /* Show the buffer */
    SET_BLEND(0);
    RENDER_TO_DRAWABLE(0, 0);

}
/*}}}*/
