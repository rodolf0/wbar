#include "EvasWindow.h"

EvasWindow::EvasWindow(size_t x, size_t y, size_t w, size_t h) :
    pos_x(x), pos_y(y), width(w), height(h) {

  this->d = XOpenDisplay(NULL);
  this->w = XCreateSimpleWindow(this->d, DefaultRootWindow(this->d),
                                this->pos_x, this->pos_y,
                                this->width, this->height,
                                0 /* border width */, 0 /* border pix */, 0);

  Evas_Engine_Info_XRender_X11 *einfo;

  evas_init();
  this->evas = evas_new();
  evas_output_method_set(this->evas, evas_render_method_lookup("xr"));
  evas_output_size_set(this->evas, this->width, this->height);
  evas_output_viewport_set(this->evas, 0, 0, this->width, this->height);
  einfo = (Evas_Engine_Info_XRender_X11*)evas_engine_info_get(this->evas);

  einfo->info.display = this->d;
  einfo->info.visual = DefaultVisual(this->d, DefaultScreen(this->d));
  einfo->info.colormap = DefaultColormap(this->d, DefaultScreen(this->d));
  einfo->info.drawable = this->w;
  einfo->info.depth = DefaultDepth(this->d, DefaultScreen(this->d));
  evas_engine_info_set(this->evas, (Evas_Engine_Info*)einfo);
}

EvasWindow::~EvasWindow() {
  evas_shutdown();
  XDestroyWindow(this->d, this->w);
  XCloseDisplay(this->d);
}

/* vim: set sw=2 sts=2 : */
