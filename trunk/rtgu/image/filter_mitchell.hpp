#ifndef BOOST_GIL_FILTER_MITCHELL_HPP
#define BOOST_GIL_FILTER_MITCHELL_HPP

// Filters adapted from nvidia-texture-tools,  src/nvimage/Filter.h,cpp @ rev622 (code is in the public domain -- castanyo@yahoo.es)

#include <cmath>

namespace boost { namespace gil {

struct filter_mitchell
{
protected:
  const float p0, p2, p3;
  const float q0, q1, q2, q3;

public:

  filter_mitchell(float b = 1.0f / 3.0f, float c = 1.0f / 3.0f)
  : p0((6.0f -  2.0f * b) / 6.0f)
  , p2((-18.0f + 12.0f * b + 6.0f * c) / 6.0f)
  , p3((12.0f - 9.0f * b - 6.0f * c) / 6.0f)
  , q0((8.0f * b + 24.0f * c) / 6.0f)
  , q1((-12.0f * b - 48.0f * c) / 6.0f)
  , q2((6.0f * b + 30.0f * c) / 6.0f)
  , q3((-b - 6.0f * c) / 6.0f)
  {}

  float width() const { return 2.0f; }

  float evaluate(float x) const
  { 
    x = std::fabs(x);
    if (x < 1.0f) return p0 + x * x * (p2 + x * p3);
    if (x < 2.0f) return q0 + x * (q1 + x * (q2 + x * q3));
    return 0.0f;
  }
};

} }   // namespace boost::gil

#endif
