#include <cmath>
#include "LayoutStrategy.h"

LayoutStrategy::~LayoutStrategy() {}

WaveLayout::WaveLayout(int num_widgets, int widget_size,
                       int num_anim, float zoom_factor, float jump_factor) :
    widget_size(widget_size), widget_dist(1), num_animated(num_anim),
    zoom_factor(zoom_factor), jump_factor(jump_factor), position(), bounds() {

  for (int i = 0; i < num_widgets; i++) {
    Point p(widget_growth()/2.0 + widget_unit() * (i + 0.5),
            bar_y() + (int)(widget_size * 0.125));
    position.push_back(p);
    bounds.push_back(Rect(p.x, p.y, widget_size, widget_size));
  }
}


void WaveLayout::unfocus() {
  for (int i = 0; i < position.size(); i++) {
    Rect &w = bounds[i];
    w.x = position[i].x;
    w.y = position[i].y;
    w.width = w.height = widget_size;
  }
}


void WaveLayout::focus(const Point &p) {
  const float x = (p.x - (widget_growth() + widget_unit())/2.0);
  const int focused = x / widget_unit();
  float rx = x - widget_unit()/2.0; // widget-space relative x

  for (int i = 0; i < bounds.size(); i++, rx -= widget_unit()) {
    Rect &w = bounds[i];
    if (std::abs((int)i - focused) > side_num_anim()) {
      w.width = w.height = widget_size;
      w.x = position[i].x + widget_growth() * ((int)i<focused?-1:1) / 2.0;
      w.y = position[i].y;
    } else {
      float cos2 = std::cos(rx * M_PI/widget_unit()/num_animated);
      w.width = w.height = widget_size * (1.0 + (zoom_factor-1.0) * cos2);

      w.x = position[i].x - (w.width - widget_size) / 2.0 -
            rx * widget_growth() / widget_unit() / num_animated;

      w.y = position[i].y - jump_factor * (w.height - widget_size);
    }
  }
}


int WaveLayout::widgetAt(const Point &p) const {
  for (std::vector<Rect>::const_iterator widget = bounds.begin();
       widget != bounds.end(); widget++) {
    if (p.x >= widget->x && p.x < widget->x + widget->width)
      return std::distance(widget, bounds.begin());
  }
  return -1;
}


bool WaveLayout::atHoverZone(const Point &p) const {
  const float relativex = (p.x - (widget_growth() + widget_unit())/2.0);
  return (relativex >= 0.0 && relativex < bounds.size() * widget_unit());
}


#define MARGEN 4
Size WaveLayout::frameSize() const {
  return Size(widget_growth() + widget_unit() * (bounds.size() + 1),
              bar_height() + 2 * MARGEN + (int)(_upgrowth() + _dngrowth()));
}


const Rect & WaveLayout::widgetLayout(int idx) const {
	return bounds[idx];
}


const Rect & WaveLayout::dockLayout() const {
  //TODO: implement dock layout
}


///// WaveLayout private helpers ///// TODO: verify inlining or cache

int WaveLayout::widget_unit() const {
  return widget_size + widget_dist;
}

int WaveLayout::side_num_anim() const {
  return (num_animated - 1) / 2;
}

int WaveLayout::widget_growth() const {
  // computed by taking the size of evenly distributed widgets (num_animated)
  // inside a cos-function wrapper: shifted and scaled so that scale ranges
  // from -pi/2 to pi/2 holding num_animated widgets with spacing (widget_unit)
  float wo = 0.0;
  for (float i = 0.0; i < num_animated; i+=1.0)
    wo += std::sin(M_PI * (i + 0.5) / (float)num_animated);

  return widget_unit() * (zoom_factor < 1.0 ? 0.0 : zoom_factor - 1.0) *  wo;
  //return widget_unit() * (zoom_factor * wo - (float)num_animated);
}

int WaveLayout::bar_height() const {
  return widget_size * 1.25;
}

int WaveLayout::bar_x() const {
  //return widget_offset();
}

int WaveLayout::bar_y() const {
  return MARGEN + (int)_upgrowth();
}

float WaveLayout::_upgrowth() const {
  const float ug = widget_size * ((zoom_factor - 1.0) * jump_factor - 0.125);
  return (ug > 0.0 ? ug : 0.0);
}

float WaveLayout::_dngrowth() const {
  const float dg =
           widget_size * ((zoom_factor - 1.0) * (1.0 - jump_factor) - 0.125);
  return (dg > 0.0 ? dg : 0.0);
}

/* vim: set sw=2 sts=2 : */
