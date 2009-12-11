#ifndef BOOST_GIL_FILTER_BSPLINE_HPP
#define BOOST_GIL_FILTER_BSPLINE_HPP

// Filters adapted from nvidia-texture-tools,  src/nvimage/Filter.h @ rev622 (code is in the public domain -- castanyo@yahoo.es)

#include <cmath>

namespace boost { namespace gil {

struct filter_bspline
{
  filter_bspline() {};

  float width() const { return 2.0f; }

  float evaluate(float x) const {
    x = std::fabs(x);
    if (x < 1.0f) return (4.0f + x * x * (-6.0f + x * 3.0f)) / 6.0f;
    if (x < 2.0f) { 
      float t = 2.0f - x;
      return (t * t * t) / 6.0f;
    }
    return 0.0f;
  }

};

} }   // namespace boost::gil

#endif
