#include "CanvasEngine.h"
//#include <Evas_Engine_GL_X11.h>
#include <Evas_Engine_Software_X11.h>


CanvasEngine *CanvasEngine::instance = NULL;

CanvasEngine & CanvasEngine::get() {
  if (!instance)
    throw "CanvasEngine must be initialized first";
  return *CanvasEngine::instance;
}


void CanvasEngine::init(Xwindow &frame) {
  if (instance)
    throw "CanvasEngine was already initialized";
  instance = new CanvasEngine(frame);
}


CanvasEngine::CanvasEngine(Xwindow &frame) {
  Evas_Engine_Info_Software_X11 *einfo;
  int32_t method;

  evas_init();

  if ((method = evas_render_method_lookup("software_x11")) <= 0)
    throw "ERROR: evas was not compiled with 'software_x11' engine!";

  if (!(canvas = evas_new()))
    throw "ERROR: could not instantiate new evas canvas.";

  evas_output_method_set(canvas, method);
  evas_output_size_set(canvas, frame.width(), frame.height());
  evas_output_viewport_set(canvas, 0, 0, frame.width(), frame.height());

  if (!(einfo = (Evas_Engine_Info_Software_X11 *)evas_engine_info_get(canvas))) {
    evas_free(canvas);
    throw "ERROR: could not get evas engine info!";
  }

  einfo->info.connection = frame.getDisplay();
  einfo->info.visual = frame.getVisual();
  einfo->info.colormap = frame.getColormap();
  einfo->info.drawable = frame.getWindow();
  einfo->info.depth = frame.getDepth();
	einfo->info.destination_alpha = 1;
	einfo->info.debug = 0;

  if (!evas_engine_info_set(canvas, (Evas_Engine_Info *)einfo)) {
    evas_free(canvas);
    throw "ERROR: failed to set evas engine info.";
  }
}


CanvasEngine::~CanvasEngine() {
  //TODO: evas_object_del(img)
  evas_free(canvas);
}


void CanvasEngine::addImage(const std::string &path, const Layout *l) {
  Evas_Object *img = evas_object_image_filled_add(canvas);
  evas_object_image_file_set(img, path.c_str(), NULL);
  if (evas_object_image_load_error_get(img) != EVAS_LOAD_ERROR_NONE)
    throw "ERROR: failed to load image.";
	evas_object_image_smooth_scale_set(img, EINA_FALSE);
	evas_object_image_alpha_set(img, EINA_TRUE);
  image_objects[img] = l;
  evas_object_show(img);
}


void CanvasEngine::render() {
  for (std::map<Evas_Object *, const Layout *>::iterator img = image_objects.begin();
       img != image_objects.end(); img++) {
    img->second->transform(img->first);
  }
  evas_render(canvas);
}
