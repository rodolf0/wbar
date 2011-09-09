#include <cmath>
#include "LayoutStrategy.h"

WaveLayout::WaveLayout(size_t widget_size, size_t num_anim,
                       float zoom_factor, float jump_factor) :
    widget_size(widget_size), widget_dist(1), num_animated(num_anim),
    zoom_factor(zoom_factor), jump_factor(jump_factor), position(), bounds() {
}

void WaveLayout::resize(size_t num) {
  position.clear();
  bounds.clear();
  for (size_t i = 0; i < num; i++) {
    Point p(widget_offset() + widget_size/2 + i * widget_unit(),
            bar_y() + (size_t)(widget_size * 0.125));
    position.push_back(p);
    bounds.push_back(Rect(p.x, p.y, widget_size, widget_size));
  }
}

void WaveLayout::unfocus() {
  for (size_t i = 0; i < position.size(); i++) {
    Rect &w = bounds[i];
    w.x = position[i].x;
    w.y = position[i].y;
    w.width = w.height = widget_size;
  }
}

void WaveLayout::focus(const Point &p) {
  const int relativex = (p.x - (widget_offset()+widget_size/2.0));
  const int focused = relativex / widget_unit();
  const float scl_b = M_PI / widget_unit() / num_animated;
  int rx = relativex - widget_unit()/2;

  for (size_t i = 0; i < bounds.size(); i++, rx -= widget_unit()) {
    Rect &w = bounds[i];
    if (std::abs(i - focused) > side_num_anim()) {
      w.width = w.height = widget_size;
      w.x = position[i].x + side_num_anim()*scaled_unit()*((int)i<focused?1:-1);
      w.y = position[i].y;
    } else {
      w.width = w.height =
        widget_size * (1.0 + (zoom_factor-1.0) * std::cos(scl_b * rx));
      w.x = position[i].x + side_num_anim()*scaled_unit() * std::sin(-scl_b*rx);
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
  const float relativex = (p.x - (widget_offset() + widget_size/2.0));
  return (relativex > 0.0 && relativex < bounds.size() * widget_unit());
}


// WaveLayout private helpers

size_t WaveLayout::widget_unit() const {
  return widget_size + widget_dist;
}

size_t WaveLayout::side_num_anim() const {
  return (num_animated - 1) / 2;
}

size_t WaveLayout::widget_offset() const {
  return side_num_anim() * widget_unit() *
         ((zoom_factor > 1.0) ? (zoom_factor - 1.0) : 0.5);
}

float WaveLayout::scaled_unit() const {
  return widget_unit() * ((zoom_factor > 1.0)?(zoom_factor - 1.0):zoom_factor);
}

size_t WaveLayout::bar_width() const {
  return widget_unit() * (bounds.size() + 1) - 2 * widget_dist;
}

size_t WaveLayout::bar_height() const {
  return widget_size * 1.25;
}

size_t WaveLayout::bar_x() const {
  return widget_offset();
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

#define MARGEN 4
Size WaveLayout::frameSize() const {
  return Size(bar_width() + 2 * widget_offset(),
              bar_height() + 2 * MARGEN + (size_t)(_upgrowth() + _dngrowth()));
}

size_t WaveLayout::bar_y() const {
  return MARGEN + (size_t)_upgrowth();
}

/* vim: set sw=2 sts=2 : */
