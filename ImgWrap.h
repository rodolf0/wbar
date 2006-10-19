#ifndef _IMGWRAP_H_
#define _IMGWRAP_H_

#include <X11/Xlib.h>
#include <Imlib2.h>

#define _image Imlib_Image
#define _font Imlib_Font

#define SET_BLEND(x) \
    imlib_context_set_blend(x) 

#define LOAD_IMAGE(path) \
    imlib_load_image(path)

#define USE_IMAGE(x) \
    imlib_context_set_image(x)

#define IMAGE_WIDTH() \
    imlib_image_get_width()

#define IMAGE_HEIGHT() \
    imlib_image_get_height()

#define IMAGE_TRANSPOSE() \
    imlib_image_flip_diagonal()

#define IMAGE_ENABLE_ALPHA(x) \
    imlib_image_set_has_alpha(x)

#define FREE_IMAGE(x) \
    do{ imlib_context_set_image(x); \
	imlib_free_image(); }while(0)

#define USE_DRAWABLE(x) \
    imlib_context_set_drawable(x)

#define IMAGE_FROM_DRAWABLE(x, y, w, h) \
    imlib_create_image_from_drawable(0, x, y, w, h, 1)

#define CLONE_IMAGE() \
    imlib_clone_image()

#define CREATE_IMAGE(w, h) \
    imlib_create_image(w, h)

#define BLEND_IMAGE(img, sx, sy, sw, sh, dx, dy, dw, dh) \
    imlib_blend_image_onto_image(img, 0, sx, sy, sw, sh, dx, dy, dw, dh)

#define RENDER_TO_DRAWABLE(x, y) \
    imlib_render_image_on_drawable(x, y)

#define INIT_IMLIB(disp, vis, cm, draw, cache) \
    do{ imlib_context_set_display(disp); \
	imlib_context_set_visual(vis); \
	imlib_context_set_colormap(cm); \
	imlib_context_set_drawable(draw); \
	imlib_set_cache_size(cache); }while(0)

#define ADD_FONT_PATH(x) \
    imlib_add_path_to_font_path(x)

#define LOAD_FONT(x) \
    imlib_load_font(x)

#define USE_FONT(x) \
    imlib_context_set_font(x)

#define FREE_FONT(x) \
    do{ imlib_context_set_font(x); \
	imlib_free_font(); }while(0)

#endif /* _IMGWRAP_H_ */
