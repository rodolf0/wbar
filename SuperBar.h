#ifndef _SUPERBAR_H_
#define _SUPERBAR_H_

#include "ImgWrap.h"
#include "Bar.h"

class SuperBar : public Bar{
    protected:
	Imlib_Filter colorFilter;
	Imlib_Filter fAlfaUnfocus;
	_font font;

	/* Icon selection */
	/* 0: none 1: hovered, 2: others, 3: all */
	int filtSel;

	int filtRed;
	int filtGreen;
	int filtBlue;
	int filtAlfa;

	/* Alfa */
	int unfocusAlfa;
	int barAlfa;

	void unfocus();

	void initFilters();

	void render();

    public:
	SuperBar(XWin *win, std::string barImg, std::string barFont, int iSize, int iDist, 
		float zFactor, float jFactor, int bOrient, int bPosition, int nAnim,
		int barAlfa, int unfocusAlfa, int filtSel, int filtCol);

	~SuperBar();

	void addIcon(std::string path, std::string comm, std::string text);
};


#endif /* _SUPERBAR_H_ */
