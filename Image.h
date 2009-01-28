#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <Imlib2.h>
#include <X11/Xlib.h>
#include <string>
#include <iostream>

class Image {
    public:
        //Image(const char *path) = 0;

        int width() const;
        int height() const;

        Image& subImage(int x, int y, int w, int h);

        // Blend
        Image& operator |= (const Image &i);
        // Paste
        Image& operator += (const Image &i);
        // Clone
        Image& operator =  (const Image &i);

}; 

class ImlibImage : public Image {
    public:

        static void  InitImlib(Display *d, Visual *v, Colormap c, Drawable dw, int cache) {
            imlib_context_set_display(d);
            imlib_context_set_visual(v);
            imlib_context_set_colormap(c);
            imlib_context_set_drawable(dw);
            imlib_set_cache_size(cache);
        }

        ImlibImage(const char *path) : 
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

        ImlibImage(Drawable src_d, int x, int y, int w, int h) :
                sx(0), sy(0), sw(w), sh(h),
                ox(0), oy(0), ow(h), oh(h) {
            Drawable d = imlib_context_get_drawable();

            imlib_context_set_drawable(src_d);
            _img = imlib_create_image_from_drawable(0, x, y, w, h, 1);

            imlib_context_set_drawable(d);
        }

        ImlibImage(int w, int h) : 
            sx(0), sy(0), sw(w), sh(h),
            ox(0), oy(0), ow(w), oh(h) {
            _img = imlib_create_image(w, h);
        }

        ~ImlibImage() {
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

        ImlibImage& full() {
            sx = 0; sy = 0; sw = ow; sh = oh;
            return *this;
        }

        ImlibImage& subImage(int x, int y, int w, int h) {
            sx = x; sy = y; sw = w; sh = h;
            return *this;
        }

        // Blend image
        ImlibImage& operator |= (const ImlibImage &i) {
            Imlib_Image t = imlib_context_get_image();
            char b;

            if( !(b = imlib_context_get_blend()) )
                imlib_context_set_blend(1);

            imlib_context_set_image(_img);
            imlib_blend_image_onto_image(i._img, 0, i.sx, i.sy, i.sw, i.sh,
                sx, sy, sw, sh);

            imlib_context_set_blend(b);
            imlib_context_set_image(t);

            return *this;
        }

        // Add image (no blending)
        ImlibImage& operator += (const ImlibImage &i) {
            Imlib_Image t = imlib_context_get_image();
            char b;

            if( !(b = imlib_context_get_blend()) )
                imlib_context_set_blend(0);

            imlib_context_set_image(_img);
            imlib_blend_image_onto_image(i._img, 0, i.sx, i.sy, i.sw, i.sh,
                sx, sy, sw, sh);

            imlib_context_set_blend(b);
            imlib_context_set_image(t);

            return *this;
        }

        ImlibImage& operator =  (const ImlibImage &i) {
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
                imlib_context_set_blend(0);

            imlib_render_image_on_drawable(0, 0);

            imlib_context_set_blend(b);
            imlib_context_set_drawable(d);
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
