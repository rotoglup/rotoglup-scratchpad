#ifndef BOOST_GIL_FILTER_CUBIC_HPP
#define BOOST_GIL_FILTER_CUBIC_HPP

// Filters adapted from nvidia-texture-tools,  src/nvimage/Filter.h @ rev622 (code is in the public domain -- castanyo@yahoo.es)

#include <cmath>

namespace boost { namespace gil {

struct filter_cubic
{
  filter_cubic() {};

  float width() const { return 1.0f; }

  float evaluate(float x) const {
    // f(t) = 2|t|^3 - 3|t|^2 + 1, -1 <= t <= 1
    x = std::fabs(x);
    if (x < 1.0f) return((2.0f * x - 3.0f) * x * x + 1.0f);
    return 0.0f;
  }

};

} }   // namespace boost::gil

#endif
