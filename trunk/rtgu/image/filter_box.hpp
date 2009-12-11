#ifndef BOOST_GIL_FILTER_BOX_HPP
#define BOOST_GIL_FILTER_BOX_HPP

// Filters adapted from nvidia-texture-tools,  src/nvimage/Filter.h @ rev622 (code is in the public domain -- castanyo@yahoo.es)

#include <cmath>

namespace boost { namespace gil {

struct filter_box
{
  float const f_width;

  filter_box() : f_width(0.5f) {};
  explicit filter_box(float width) : f_width(width) {};

  float width() const { return f_width; }

  float evaluate(float x) const {
    if (std::fabs(x) <= f_width) return 1.0f;
    else return 0.0f;
  }

};

} }   // namespace boost::gil

#endif
