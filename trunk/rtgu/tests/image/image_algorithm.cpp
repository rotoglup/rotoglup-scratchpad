#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <rtgu/image/image.hpp>
#include <rtgu/image/algorithm.hpp>

#include "test_helpers.hpp"

namespace img = rtgu::image;

struct DefaultValues
{
  DefaultValues()
  {
  }

  ~DefaultValues()
  {
  }
};


TEST_FIXTURE(DefaultValues, PremultiplyAlphaBGRA8)
{
  img::image_bgra8 image(1, 1);
  
  view(image)(0, 0) = boost::gil::bgra8_pixel_t(128, 128, 128, 128);

  img::premultiply_alpha( view(image) );

  CHECK_EQUAL( boost::gil::bgra8_pixel_t(64, 64, 64, 128), view(image)(0, 0) );
}

TEST_FIXTURE(DefaultValues, PremultiplyAlphaRGBA32f)
{
  img::image_rgba32f image(1, 1);
  
  view(image)(0, 0) = boost::gil::rgba32f_pixel_t(0.5f, 0.5f, 0.5f, 0.5f);

  img::premultiply_alpha( view(image) );

  CHECK_EQUAL( boost::gil::rgba32f_pixel_t(0.25f, 0.25f, 0.25f, 0.5f), view(image)(0, 0) );
}

TEST_FIXTURE(DefaultValues, VerticalFlip_Even)
{
  img::image_rgba32f image(1, 2);

  boost::gil::rgba32f_pixel_t const A(0.5f, 0.5f, 0.5f, 0.5f);
  boost::gil::rgba32f_pixel_t const B(0.5f, 0.5f, 0.5f, 1.0f);
  
  view(image)(0, 0) = A;
  view(image)(0, 1) = B;

  img::vertical_flip( view(image) );

  CHECK_EQUAL( B, view(image)(0,0) );
  CHECK_EQUAL( A, view(image)(0,1) );
}

TEST_FIXTURE(DefaultValues, VerticalFlip_Odd)
{
  img::image_rgba32f image(1, 3);

  boost::gil::rgba32f_pixel_t const A(0.5f, 0.5f, 0.5f, 0.0f);
  boost::gil::rgba32f_pixel_t const B(0.5f, 0.5f, 0.5f, 0.5f);
  boost::gil::rgba32f_pixel_t const C(0.5f, 0.5f, 0.5f, 1.0f);
  
  view(image)(0, 0) = A;
  view(image)(0, 1) = B;
  view(image)(0, 2) = C;

  img::vertical_flip( view(image) );

  CHECK_EQUAL( C, view(image)(0,0) );
  CHECK_EQUAL( B, view(image)(0,1) );
  CHECK_EQUAL( A, view(image)(0,2) );
}

int main(int, char const *[])
{
  return UnitTest::RunAllTests();
}
