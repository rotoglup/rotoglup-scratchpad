#ifndef BOOST_GIL_FILTER_KAISER_HPP
#define BOOST_GIL_FILTER_KAISER_HPP

// Filters adapted from nvidia-texture-tools,  src/nvimage/Filter.h,cpp @ rev622 (code is in the public domain -- castanyo@yahoo.es)

#include <cmath>
#include "filter_sinc.hpp"

namespace boost { namespace gil {

struct filter_kaiser
{
  float f_width;
  float f_alpha, f_stretch;

  explicit filter_kaiser(float width, float alpha=4.0f, float stretch=1.0f)
  : f_width(width)
  , f_alpha(alpha)
  , f_stretch(stretch)
  {}

  float width() const { return f_width; }

  float evaluate(float x) const { 
    const float sinc_value = filter_sinc::sincf(filter_sinc::pi() * x * f_stretch);
    const float t = x / f_width;
    if ((1.f - t * t) >= 0.f) return sinc_value * bessel0(f_alpha * std::sqrt(1.f - t * t)) / bessel0(f_alpha);
    else return 0.f;
  }

  // Bessel function of the first kind from Jon Blow's article.
  // http://mathworld.wolfram.com/BesselFunctionoftheFirstKind.html
  // http://en.wikipedia.org/wiki/Bessel_function
  static float bessel0(float x)
  {
    const float EPSILON_RATIO = 1e-6f;
    float xh, sum, pow, ds;
    int k;

    xh = 0.5f * x;
    sum = 1.0f;
    pow = 1.0f;
    k = 0;
    ds = 1.0;
    while (ds > sum * EPSILON_RATIO) {
      ++k;
      pow = pow * (xh / k);
      ds = pow * pow;
      sum = sum + ds;
    }

    return sum;
  }
};

} }   // namespace boost::gil

#endif
