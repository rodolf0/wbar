#include "CanvasEngine.h"


CanvasEngine & CanvasEngine::get() {
  return CanvasEngine::canvas_engine;
}


    //Xwindow(Rect(0, 0, 1, 1),

//#include <Evas_Engine_GL_X11.h>

//Evas * createCanvas(size_t width, size_t height) {

  //Evas *canvas;
  //Evas_Engine_Info_GL_X11 *einfo;
  //int32_t method;

  //if ((method = evas_render_method_lookup("software_x11")) <= 0)
    //throw "ERROR: evas was not compiled with 'xrender' engine!";

  //if (!(canvas = evas_new()))
    //throw "ERROR: could not instantiate new evas canvas.";

  //evas_output_method_set(canvas, method);
  //evas_output_size_set(canvas, width, height);
  //evas_output_viewport_set(canvas, 0, 0, width, height);

  //if (!(einfo = (Evas_Engine_Info_GL_X11 *)evas_engine_info_get(canvas))) {
    //evas_free(canvas);
    //throw "ERROR: could not get evas engine info!";
  //}

  //einfo->info.display = d;
  //einfo->info.drawable = w;

  //evas_engine_info_set(canvas, (Evas_Engine_Info *)einfo);

  //return canvas;
//}
