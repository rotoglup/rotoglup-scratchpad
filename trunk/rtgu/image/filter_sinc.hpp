#ifndef BOOST_GIL_FILTER_SINC_HPP
#define BOOST_GIL_FILTER_SINC_HPP

// Filters adapted from nvidia-texture-tools,  src/nvimage/Filter.h,cpp @ rev622 (code is in the public domain -- castanyo@yahoo.es)

#include <cmath>

namespace boost { namespace gil {

struct filter_sinc
{
  float f_width;

  explicit filter_sinc(float width) : f_width(width) {}

  float width() const { return f_width; }

  float evaluate(float x) const { 
    return filter_sinc::sincf(pi() * x);
  }

  inline static float pi()
  {
    static const float PI = 3.141592653589793238462643f;      // TODO, use some real PI, would add a dependency though
    return PI;
  }

  // Sinc function.
  inline static float sincf(const float x) {
    static const float EPSILON = 0.0001f;
    if (std::fabs(x) < EPSILON) {
      return 1.0f + x*x*(-1.0f/6.0f + x*x*1.0f/120.0f);
    }
    else {
      return std::sin(x) / x;
    }
  }
};

} }   // namespace boost::gil

#endif
