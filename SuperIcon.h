#ifndef _SUPERICON_H_
#define _SUPERICON_H_

#include "ImgWrap.h"
#include "Icon.h"

class SuperBar;

class SuperIcon : public Icon{
    protected:
	_image icon_color;

	int textW, textH;
	std::string text;
	
	friend class SuperBar;

    public:
	SuperIcon(std::string iconImg, std::string cmd, std::string txt, 
		int x, int y, int tw, int th);
	~SuperIcon();

};

#endif /* _SUPERICON_H_ */
