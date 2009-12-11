#ifndef BOOST_GIL_FILTER_TRIANGLE_HPP
#define BOOST_GIL_FILTER_TRIANGLE_HPP

// Filters adapted from nvidia-texture-tools,  src/nvimage/Filter.h @ rev622 (code is in the public domain -- castanyo@yahoo.es)

#include <cmath>

namespace boost { namespace gil {

struct filter_triangle
{
  float const f_width;

  filter_triangle() : f_width(1.0f) {};
  explicit filter_triangle(float width) : f_width(width) {};

  float width() const { return f_width; }

  float evaluate(float x) const {
    x = std::fabs(x);
    if (x < f_width) return f_width - x;
    return 0.0f;
  }

};

} }   // namespace boost::gil

#endif
