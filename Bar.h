#ifndef _BAR_H_
#define _BAR_H_

#include <vector>
#include <string>
#include "ImgWrap.h"
#include "XWin.h"
#include "Icon.h"

class Bar{
    protected:
	/* Drawing buffs */
	_image buffer;
	_image barback;
	_image bar;
	
	/* reference to the container window */
	XWin *window;

	std::vector<Icon*> icons;

	/* dist. between */ 
	int icon_dist;
	/* icon size */
	int icon_size;
	/* how many icons move */
	int icon_anim;
	/* jump factor on hover */
	float jump_factor;
	/* zoom factor on hover */
	float zoom_factor;

	/* fast math constants */
	float b_scl_d;
	float b_scl_a;
	float b_scl_b;
	float b_pos_m;
	float b_dd;
	float icon_offset;
	int icon_unit;
	int icon_ansd; // anim aside icons

	/* bar position in window */
	int x, y;
	/* original bar dimensions */
	int owidth, oheight;
	/* actual dimensions */
	int width, height;
	int orientation;
	int position;
	
	/* actual zoomed icon */
	int zoomed_icon;
	/* is bar focused? */
	int focused;

	/* set bar */
	void focus();
	virtual void unfocus();

	void iconPress(int i_num, int offs);

	void transform(int num, int off);
	virtual void render();

	void acquireBack();
	void drawBack();
	void cleanBack();

	void scale(bool updateBG = true);

	int iconNumber(int mouse_x);
	int iconOffset(int mouse_x);
	
    public:

	Bar(XWin *win, std::string barImg, int iSize, int iDist, float zFactor, 
		float jFactor, int bOrient, int bPosition, int nAnim);

	virtual ~Bar();

	/* add an Icon */
	void addIcon(std::string path, std::string comm);

	/* Icon information */
	int iconIndex(int mouse_x);
	std::string iconCommand(int i_num);

	/* Focus & unfocus events */
	void refresh(int mouse_x = -1);

	/* Icon press events */
	void iconDown(int i_num);
	void iconUp(int i_num);
	void setPosition(std::string p);

	//void setZoom(float zoomf);
	//float getZoom();
};

#endif /* _BAR_H_ */
