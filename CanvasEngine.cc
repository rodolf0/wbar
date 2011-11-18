#include <typeinfo>
#include "CanvasEngine.h"
#include <Evas_Engine_Software_X11.h>
//#include <Evas_Engine_GL_X11.h>


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


CanvasEngine::CanvasEngine(Xwindow &frame) : canvas(NULL), widgets() {
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


void CanvasEngine::resize(const Size &s) {
  evas_output_size_set(canvas, s.x, s.y);
  evas_output_viewport_set(canvas, 0, 0, s.x, s.y);
}


CanvasEngine::~CanvasEngine() {
  // evas widget objects are destructed on the CanvasWidget destructor
  widgets.clear();
  evas_free(canvas);
}


CanvasWidget & CanvasEngine::addWidget(
    const std::string &path, const Layout &layout) {
  Evas_Object *image = evas_object_image_filled_add(canvas);
  evas_object_image_file_set(image, path.c_str(), NULL);
  if (evas_object_image_load_error_get(image) != EVAS_LOAD_ERROR_NONE)
    throw "ERROR: failed to load image.";
  evas_object_image_smooth_scale_set(image, EINA_TRUE);
  evas_object_image_alpha_set(image, EINA_TRUE);
  evas_object_show(image);
  widgets.push_back(new CanvasWidget(image, layout));
  return *widgets.back();
}


void CanvasEngine::render() const {
  for (std::list<CanvasWidget *>::const_iterator w = widgets.begin();
       w != widgets.end(); w++) {
    (*w)->render();
  }
  evas_render(canvas);
}


/////////////////////// CanvasWidget /////////////////////


CanvasWidget::~CanvasWidget() {
  if (layout)
    delete layout;
  layout = NULL;
  if (image)
    evas_object_del(image);
  image = NULL;
}


CanvasWidget::CanvasWidget(Evas_Object *img, const Layout &l) :
    image(img), layout(NULL) {

  if (typeid(l) == typeid(RectLayout)) {
    layout = new RectLayout(dynamic_cast<const RectLayout &>(l));
  } else if (typeid(l) == typeid(Layout3D)) {
    layout = new Layout3D(dynamic_cast<const Layout3D &>(l));
  } else {
    throw "CanvasWidget: couldn't identify virtual layout.";
  }
}


void CanvasWidget::render() const {
  layout->transform(image);
}


void CanvasWidget::setFrame(const Border &b) {
  evas_object_image_border_set(image, b.left, b.right, b.top, b.bottom);
}


///////////////////// Layouts ////////////////////////////


Layout::~Layout() {}

RectLayout::RectLayout(const Rect &r) : layout(r) {}

void RectLayout::transform(Evas_Object *img) const {
  evas_object_resize(img, layout.width, layout.height);
  evas_object_move(img, layout.x, layout.y);
}

Layout3D::Layout3D(const Rect3D &r) : layout(r) {
}

void Layout3D::transform(Evas_Object *img) const {
}

