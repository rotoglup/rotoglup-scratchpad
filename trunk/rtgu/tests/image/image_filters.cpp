#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <rtgu/image/filter_all.hpp>

namespace bgil = boost::gil;

//----------------------------------------------------------------------------

#define TEST_EVAL_ZERO_FILTER_SIZED(FTYPE, FWIDTH) \
  TEST(eval_zero_sized_##FTYPE) \
{ \
  boost::gil::FTYPE f(FWIDTH);  \
  float w = f.width();  \
  \
  CHECK_EQUAL( 1.f, f.evaluate(0.0f) );  \
}

#define TEST_EVAL_ZERO_FILTER(FTYPE) \
  TEST(eval_zero_##FTYPE) \
{ \
  boost::gil::FTYPE f;  \
  float w = f.width();  \
  \
  CHECK_EQUAL( 1.f, f.evaluate(0.0f) );  \
}

TEST_EVAL_ZERO_FILTER(filter_box)
// N/A // TEST_EVAL_ZERO_FILTER(filter_bspline)
TEST_EVAL_ZERO_FILTER(filter_cubic)
TEST_EVAL_ZERO_FILTER_SIZED(filter_kaiser, 3.0f)
TEST_EVAL_ZERO_FILTER(filter_lanczos)
// N/A // TEST_EVAL_ZERO_FILTER(filter_mitchell)
// N/A // TEST_EVAL_ZERO_FILTER(filter_quadratic)
TEST_EVAL_ZERO_FILTER_SIZED(filter_sinc, 3.0f)
TEST_EVAL_ZERO_FILTER(filter_triangle)

//----------------------------------------------------------------------------

#define TEST_SYMETRIC_FILTER_SIZED(FTYPE, FWIDTH) \
  TEST(symetry_sized_##FTYPE) \
{ \
  boost::gil::FTYPE f(FWIDTH);  \
  float w = f.width();  \
  \
  CHECK_EQUAL( f.evaluate(0.0f*w), f.evaluate(-0.0f*w) );  \
  CHECK_EQUAL( f.evaluate(0.5f*w), f.evaluate(-0.5f*w) );  \
  CHECK_EQUAL( f.evaluate(1.0f*w), f.evaluate(-1.0f*w) );  \
  CHECK_EQUAL( f.evaluate(2.0f*w), f.evaluate(-2.0f*w) );  \
}

#define TEST_SYMETRIC_FILTER(FTYPE) \
  TEST(symetry_##FTYPE) \
{ \
  boost::gil::FTYPE f;  \
  float w = f.width();  \
  \
  CHECK_EQUAL( f.evaluate(0.0f*w), f.evaluate(-0.0f*w) );  \
  CHECK_EQUAL( f.evaluate(0.5f*w), f.evaluate(-0.5f*w) );  \
  CHECK_EQUAL( f.evaluate(1.0f*w), f.evaluate(-1.0f*w) );  \
  CHECK_EQUAL( f.evaluate(2.0f*w), f.evaluate(-2.0f*w) );  \
}

TEST_SYMETRIC_FILTER(filter_box)
TEST_SYMETRIC_FILTER(filter_bspline)
TEST_SYMETRIC_FILTER(filter_cubic)
TEST_SYMETRIC_FILTER_SIZED(filter_kaiser, 3.0f)
TEST_SYMETRIC_FILTER(filter_lanczos)
TEST_SYMETRIC_FILTER(filter_mitchell)
TEST_SYMETRIC_FILTER(filter_quadratic)
TEST_SYMETRIC_FILTER_SIZED(filter_sinc, 3.0f)
TEST_SYMETRIC_FILTER(filter_triangle)

//----------------------------------------------------------------------------

#define TEST_WINDOW_FILTER(FTYPE) \
TEST(window_##FTYPE) \
{ \
  boost::gil::FTYPE f;  \
  float w = f.width();  \
  \
  CHECK_EQUAL( 0.f, f.evaluate(w) );  \
}

// N/A on purpose? // TEST_WINDOW_FILTER(filter_box)
TEST_WINDOW_FILTER(filter_bspline)
TEST_WINDOW_FILTER(filter_cubic)
// N/A // TEST_WINDOW_FILTER(filter_kaiser)
TEST_WINDOW_FILTER(filter_lanczos)
TEST_WINDOW_FILTER(filter_mitchell)
TEST_WINDOW_FILTER(filter_quadratic)
// N/A // TEST_WINDOW_FILTER(filter_sinc)
TEST_WINDOW_FILTER(filter_triangle)

//----------------------------------------------------------------------------

int main(int, char const *[])
{
  return UnitTest::RunAllTests();
}
