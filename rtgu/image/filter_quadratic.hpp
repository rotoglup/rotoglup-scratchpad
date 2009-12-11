#ifndef BOOST_GIL_FILTER_QUADRATIC_HPP
#define BOOST_GIL_FILTER_QUADRATIC_HPP

// Filters adapted from nvidia-texture-tools,  src/nvimage/Filter.h @ rev622 (code is in the public domain -- castanyo@yahoo.es)

#include <cmath>

namespace boost { namespace gil {

struct filter_quadratic
{
  filter_quadratic() {};

  float width() const { return 1.5f; }

  float evaluate(float x) const {
    x = std::fabs(x);
    if( x < 0.5f ) return 0.75f - x * x;
    if( x < 1.5f ) { 
      float t = x - 1.5f;
      return 0.5f * t * t;
    }
    return 0.0f;
  }

};

} }   // namespace boost::gil

#endif
