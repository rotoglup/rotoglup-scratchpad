#ifndef BOOST_GIL_FILTER_LANCZOS_HPP
#define BOOST_GIL_FILTER_LANCZOS_HPP

// Filters adapted from nvidia-texture-tools,  src/nvimage/Filter.h,cpp @ rev622 (code is in the public domain -- castanyo@yahoo.es)

#include <cmath>
#include "filter_sinc.hpp"

namespace boost { namespace gil {

struct filter_lanczos
{
  filter_lanczos() {}

  float width() const { return 3.0f; }

  float evaluate(float x) const { 
    x = std::fabs(x);
    if (x < 3.0f) return filter_sinc::sincf(filter_sinc::pi() * x) * filter_sinc::sincf(filter_sinc::pi() * x / 3.0f);
    return 0.0f;
  }
};

} }   // namespace boost::gil

#endif
