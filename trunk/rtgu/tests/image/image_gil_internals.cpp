#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <boost/gil/gil_all.hpp>

#include <rtgu/image/image.hpp>

#include "test_helpers.hpp"

namespace gil = boost::gil;
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

TEST_FIXTURE(DefaultValues, GilFloatMaxValueIsOne)
{    
  CHECK_EQUAL( 1, gil::channel_type<gil::rgba32f_pixel_t>::type::max_value() );
}

TEST_FIXTURE(DefaultValues, GilFloatIsNotClamped)
{    
  typedef gil::image< gil::rgba32f_pixel_t, false > rgba32f_image_t;
  
  rgba32f_image_t image(1, 1);

  gil::view(image)(0, 0) = gil::rgba32f_pixel_t(2, 1, 1, 2);
  
  CHECK_EQUAL( gil::rgba32f_pixel_t(2, 1, 1, 2), gil::view(image)(0, 0) );
}

TEST_FIXTURE(DefaultValues, GilImageMemoryAccess)
{
  typedef gil::rgba32f_pixel_t pixel_t;
  typedef gil::image<pixel_t, false > image_t;

  unsigned const W = 2;

  image_t image(W, 2);

  CHECK_EQUAL( W * sizeof(pixel_t), img::get_scanline_size(image) );

  img::get_pixels_memory(image)[0] = pixel_t(2, 1, 1, 2);
  img::get_pixels_memory(image)[1] = pixel_t(2, 2, 1, 2);
  img::get_pixels_memory(image)[2] = pixel_t(2, 1, 2, 2);
  img::get_pixels_memory(image)[3] = pixel_t(2, 2, 2, 2);

  CHECK_EQUAL( pixel_t(2, 1, 1, 2), gil::view(image)(0, 0) );
  CHECK_EQUAL( pixel_t(2, 2, 1, 2), gil::view(image)(1, 0) );
  CHECK_EQUAL( pixel_t(2, 1, 2, 2), gil::view(image)(0, 1) );
  CHECK_EQUAL( pixel_t(2, 2, 2, 2), gil::view(image)(1, 1) );
}

TEST_FIXTURE(DefaultValues, GilImageMemoryNoScanlinePadding)
{
  typedef gil::rgb8_pixel_t pixel_t;
  typedef gil::image<pixel_t, false > image_t;

  unsigned const W = 3;

  image_t image(W, 2);

  CHECK_EQUAL( 1, sizeof(pixel_t) % 2 );

  CHECK_EQUAL( W * sizeof(pixel_t), img::get_scanline_size(image) );

  img::get_pixels_memory(image)[0] = pixel_t(2, 1, 1);
  img::get_pixels_memory(image)[1] = pixel_t(2, 2, 1);
  img::get_pixels_memory(image)[2] = pixel_t(2, 1, 2);
  img::get_pixels_memory(image)[3] = pixel_t(2, 2, 2);

  CHECK_EQUAL( pixel_t(2, 1, 1), gil::view(image)(0, 0) );
  CHECK_EQUAL( pixel_t(2, 2, 1), gil::view(image)(1, 0) );
  CHECK_EQUAL( pixel_t(2, 1, 2), gil::view(image)(2, 0) );
  CHECK_EQUAL( pixel_t(2, 2, 2), gil::view(image)(0, 1) );
}

TEST_FIXTURE(DefaultValues, GilRgbToRgba)
{
  // ensure that GIL adds opaque alpha value when default converting from RGB to RGBA

  typedef gil::rgb8_pixel_t rgb_pixel_t;
  typedef gil::rgba8_pixel_t rgba_pixel_t;
  
  typedef gil::image<rgb_pixel_t, false >  rgb_image_t;
  typedef gil::image<rgba_pixel_t, false > rgba_image_t;

  rgb_image_t  src_image(1, 1);
  rgba_image_t dst_image(1, 1);

  gil::view(src_image)(0, 0) = rgb_pixel_t(128, 128, 128);

  gil::copy_and_convert_pixels( view(src_image), view(dst_image) );

  CHECK_EQUAL( rgba_pixel_t(128, 128, 128, 255), gil::view(dst_image)(0, 0) );
}

TEST_FIXTURE(DefaultValues, GilRgbaToRgb)
{
  // ensure that GIL premultiplies with alpha when default converting from RGBA to RGB

  typedef gil::rgb8_pixel_t rgb_pixel_t;
  typedef gil::rgba8_pixel_t rgba_pixel_t;
  
  typedef gil::image<rgb_pixel_t, false >  rgb_image_t;
  typedef gil::image<rgba_pixel_t, false > rgba_image_t;

  rgba_image_t src_image(1, 1);
  rgb_image_t  dst_image(1, 1);

  gil::view(src_image)(0, 0) = rgba_pixel_t(128, 128, 128, 128);

  gil::copy_and_convert_pixels( view(src_image), view(dst_image) );

  CHECK_EQUAL( rgb_pixel_t(64, 64, 64), gil::view(dst_image)(0, 0) );
}

TEST_FIXTURE(DefaultValues, GilCopyCompatibleImagesWithoutConvert)
{
  typedef gil::rgba8_pixel_t   pixel_t1;
  typedef gil::rgb32f_pixel_t  pixel_t2;
  
  typedef gil::image<pixel_t1, false > image_t1;
  typedef gil::image<pixel_t2, false > image_t2;

  image_t1 src_image(1, 1);
  image_t2 dst_image(1, 1);

  gil::view(src_image)(0, 0) = pixel_t1(2, 2, 2, 2);

  gil::copy_pixels( view(src_image), view(dst_image) );

  CHECK_EQUAL( pixel_t1(2, 2, 2, 2), gil::view(src_image)(0, 0) );
  CHECK_EQUAL( pixel_t2(2, 2, 2), gil::view(dst_image)(0, 0) );
}

//----------------------------------------------------------------------------

int main(int, char const *[])
{
  return UnitTest::RunAllTests();
}
