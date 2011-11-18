#include <cmath>
#include "LayoutStrategy.h"

#define BAR_HEIGHT_FACTOR 0.5

LayoutStrategy::~LayoutStrategy() {}


WaveLayout::WaveLayout(int widget_size, int num_anim,
                       float zoom_factor, float jump_factor) :
    widget_size(widget_size), widget_dist(10), bar_margin(5),
    num_animated(num_anim), zoom_factor(zoom_factor), jump_factor(jump_factor),
    position(), bounds(),
    dock_bounds(widget_growth()/2.0, bar_y(), widget_unit(), bar_height()) { }


WaveLayout::~WaveLayout() {
  for (std::vector<Rect *>::iterator r = bounds.begin();
       r != bounds.end(); r++) delete *r;
  for (std::vector<Point *>::iterator p = position.begin();
       p != position.end(); p++) delete *p;
}


const Rect & WaveLayout::addWidget() {
  dock_bounds.width += widget_unit();
  Point *p = new Point(
    (widget_growth() + widget_dist)/2.0 + widget_unit() * (bounds.size() + 0.5),
    bar_y() + (int)(widget_size * BAR_HEIGHT_FACTOR / 2.0));
  position.push_back(p);
  bounds.push_back(new Rect(p->x, p->y, widget_size, widget_size));
  return *bounds.back();
}


void WaveLayout::unfocus() {
  for (size_t i = 0; i < position.size(); i++) {
    Rect &w = *bounds[i];
    w.x = position[i]->x;
    w.y = position[i]->y;
    w.width = w.height = widget_size;
  }
  dock_bounds.x = widget_growth() / 2.0;
  dock_bounds.width = widget_unit() * (bounds.size() + 1);
}


void WaveLayout::focus(const Point &p) {
  const float x = (p.x - (widget_growth() + widget_unit() + widget_dist)/2.0);
  const size_t focused = x / widget_unit();
  float rx = x - widget_unit()/2.0; // widget-space relative x
  const float wu_na = widget_unit() * num_animated;

  for (size_t i = 0; i < bounds.size(); i++, rx -= widget_unit()) {
    Rect &w = *bounds[i];
    if (std::abs(rx) > wu_na/2) {
      w.width = w.height = widget_size;
      w.x = position[i]->x + widget_growth() / (i<focused ? -2.0 : 2.0);
      w.y = position[i]->y;
    } else {
      w.width = w.height =
        widget_size * (1.0 + (zoom_factor-1.0) * std::cos(rx * M_PI/wu_na));
      w.x = position[i]->x - (w.width - widget_size) / 2.0 -
        rx * widget_growth() / wu_na;
      w.y = position[i]->y - jump_factor * (w.height - widget_size);
    }
  }
  dock_bounds.x = 0;
  dock_bounds.width = widget_growth() + widget_unit() * (bounds.size() + 1);
}


int WaveLayout::widgetAt(const Point &p) const {
  for (std::vector<Rect*>::const_iterator widget = bounds.begin();
       widget != bounds.end(); widget++) {
    if (p.x >= (*widget)->x && p.x < (*widget)->x + (*widget)->width)
      return std::distance(bounds.begin(), widget);
  }
  return -1;
}


bool WaveLayout::atHoverZone(const Point &p) const {
  const float x = (p.x - (widget_growth() + widget_unit() + widget_dist)/2.0);
  return (x >= 0.0 && x < bounds.size() * widget_unit() - widget_dist);
}


Size WaveLayout::frameSize() const {
  return Size(widget_growth() + widget_unit() * (bounds.size() + 1),
              bar_height() + 2 * bar_margin + (int)(_upgrowth() + _dngrowth()));
}

const Rect & WaveLayout::dockLayout() const { return dock_bounds; }


///// WaveLayout private helpers ///// TODO: memoize

int WaveLayout::widget_unit() const {
  return widget_size + widget_dist;
}

int WaveLayout::widget_growth() const {
  // computed by taking the size of evenly distributed widgets (num_animated)
  // inside a cos-function wrapper: shifted and scaled so that scale ranges
  // from -pi/2 to pi/2 holding num_animated widgets with spacing (widget_unit)
  float wo = 0.0;
  for (float i = 0.0; i < num_animated; i+=1.0)
    wo += std::sin(M_PI * (i + 0.5) / (float)num_animated);

  return widget_unit() * (zoom_factor < 1.0 ? 0.0 : zoom_factor - 1.0) *  wo;
}

int WaveLayout::bar_height() const {
  return widget_size * (1 + BAR_HEIGHT_FACTOR);
}

int WaveLayout::bar_y() const {
  return bar_margin + (int)_upgrowth();
}

float WaveLayout::_upgrowth() const {
  const float ug = widget_size *
    ((zoom_factor - 1.0) * jump_factor - BAR_HEIGHT_FACTOR/2.0);
  return (ug > 0.0 ? ug : 0.0);
}

float WaveLayout::_dngrowth() const {
  const float dg = widget_size *
    ((zoom_factor - 1.0) * (1.0 - jump_factor) - BAR_HEIGHT_FACTOR/2.0);
  return (dg > 0.0 ? dg : 0.0);
}

/* vim: set sw=2 sts=2 : */
