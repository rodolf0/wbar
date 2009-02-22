#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <Imlib2.h>
#include <X11/Xlib.h>
#include <string>
#include <iostream>

#if 0
#include <cairo.h>
#include <cairo-xlib.h>
#endif

class Image {
    public:

        static void  Init(Display *d, Visual *v, Colormap c, Drawable dw, int cache) {
            imlib_context_set_display(d);
            imlib_context_set_visual(v);
            imlib_context_set_colormap(c);
            imlib_context_set_drawable(dw);
            imlib_set_cache_size(cache);
            imlib_context_set_operation(IMLIB_OP_COPY);
        }

        Image(const char *path) : 
                sx(0), sy(0), ox(0), oy(0) {
            Imlib_Image t = imlib_context_get_image();

            if( !(_img = imlib_load_image(path)) ){
                throw path;
            }
            std::cout << "Loading: " << path << std::endl;

            imlib_context_set_image(_img);
            ow = sw = imlib_image_get_width();
            oh = sh = imlib_image_get_height();
            imlib_image_set_has_alpha(1);

            imlib_context_set_image(t);
        }

        Image(Drawable src_d, int x, int y, int w, int h) :
                sx(0), sy(0), sw(w), sh(h),
                ox(0), oy(0), ow(w), oh(h) {
            Drawable d = imlib_context_get_drawable();

            imlib_context_set_drawable(src_d);
            _img = imlib_create_image_from_drawable(0, x, y, w, h, 1);

            imlib_context_set_drawable(d);
        }

        Image(int w, int h) : 
            sx(0), sy(0), sw(w), sh(h),
            ox(0), oy(0), ow(w), oh(h) {
            _img = imlib_create_image(w, h);
        }

        ~Image() {
            Imlib_Image t = imlib_context_get_image();
            imlib_context_set_image(_img);
            imlib_free_image();
            imlib_context_set_image(t);
        }

        void setAlpha(int x) {
            Imlib_Image t = imlib_context_get_image();
            imlib_context_set_image(_img);
            imlib_image_set_has_alpha(x);
            imlib_context_set_image(t);
        }

        Image& full() {
            sx = 0; sy = 0; sw = ow; sh = oh;
            return *this;
        }

        Image& subImage(int x, int y, int w, int h) {
            sx = x; sy = y; sw = w; sh = h;
            return *this;
        }

        Image& colorClear(int r, int g, int b, int a) {
            Imlib_Image t = imlib_context_get_image();

            imlib_context_set_image(_img);
            imlib_image_clear_color(r, g, b, a);

            imlib_context_set_image(t);
            return *this;
        }

        // Blend image
        Image& operator |= (const Image &i) {
            Imlib_Image t = imlib_context_get_image();
            char b;
            if( !(b = imlib_context_get_blend()) )
                imlib_context_set_blend(1);

            imlib_context_set_image(_img);
            imlib_blend_image_onto_image(i._img, 1, i.sx, i.sy, i.sw, i.sh,
                sx, sy, sw, sh);

            imlib_context_set_blend(b);
            imlib_context_set_image(t);

            return *this;
        }

        // Add image (no blending)
        Image& operator += (const Image &i) {
            Imlib_Image t = imlib_context_get_image();
            char b;

            if( !(b = imlib_context_get_blend()) )
                imlib_context_set_blend(0);

            imlib_context_set_image(_img);
            imlib_blend_image_onto_image(i._img, 1, i.sx, i.sy, i.sw, i.sh,
                sx, sy, sw, sh);

            imlib_context_set_blend(b);
            imlib_context_set_image(t);

            return *this;
        }

        Image& operator =  (const Image &i) {
            if(&i != this) {

                Imlib_Image t = imlib_context_get_image();

                if( _img ) {
                    imlib_context_set_image(_img);
                    imlib_free_image();
                }

                imlib_context_set_image(i._img);
                _img = imlib_clone_image();
                sx = i.sx; sy = i.sy; sw = i.sw; sh = i.sh;
                ox = i.ox; oy = i.oy; ow = i.ow; oh = i.oh;

                imlib_context_set_image(t);
            }

            return *this;
        }

        void splat (Drawable dst_d) {
            Imlib_Image t = imlib_context_get_image();
            Drawable d = imlib_context_get_drawable();
            char b;

            imlib_context_set_drawable(dst_d);
            imlib_context_set_image(_img);
            if( !(b = imlib_context_get_blend()) )
                imlib_context_set_blend(1);

#if 1
            imlib_render_image_on_drawable(0, 0);
#else
            cairo_surface_t *res = cairo_image_surface_create_for_data(
                (unsigned char*)imlib_image_get_data_for_reading_only(),
                CAIRO_FORMAT_ARGB32, ow, oh, sizeof(DATA32) * ow);

            cairo_surface_t * dwb = cairo_xlib_surface_create(
                imlib_context_get_display(), dst_d, 
                imlib_context_get_visual(), ow, oh);

            // create a context for the target
            cairo_t *ctx = cairo_create(dwb);

            /*
            cairo_set_source_rgba(ctx, 1, 1, 1, 1);
            cairo_rectangle(ctx, 0, 0, ow, oh);
            cairo_fill(ctx);
            */

            cairo_set_source_surface(ctx, res, 0, 0);
            cairo_set_operator(ctx, CAIRO_OPERATOR_SOURCE);
            cairo_paint(ctx);

            cairo_destroy(ctx);
            cairo_surface_destroy(res);
            cairo_surface_destroy(dwb);
#endif

            imlib_context_set_blend(b);
            imlib_context_set_drawable(d);
            imlib_context_set_image(t);
        }

        void save(const char *path) {
            Imlib_Image t = imlib_context_get_image();

            imlib_context_set_image(_img);
            imlib_save_image(path);

            imlib_context_set_image(t);
        }


    //private:

        Imlib_Image _img;
        //
        // SubImage rect
        int sx, sy, sw, sh;
        // Original dimensions
        int ox, oy, ow, oh;
}; 


#endif // _IMAGE_H_

/* vim: set sw=4 sts=4 : */
