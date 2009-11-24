#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <boost/gil/gil_all.hpp>

#include <rtgu/image/image.hpp>

#include "test_helpers.hpp"

namespace bgil = boost::gil;
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
  CHECK_EQUAL( 1, bgil::channel_type<bgil::rgba32f_pixel_t>::type::max_value() );
}

TEST_FIXTURE(DefaultValues, GilFloatIsNotClamped)
{
  typedef bgil::image< bgil::rgba32f_pixel_t, false > rgba32f_image_t;

  rgba32f_image_t image(1, 1);

  bgil::view(image)(0, 0) = bgil::rgba32f_pixel_t(2, 1, 1, 2);

  CHECK_EQUAL( bgil::rgba32f_pixel_t(2, 1, 1, 2), bgil::view(image)(0, 0) );
}

TEST_FIXTURE(DefaultValues, GilImageMemoryAccess)
{
  typedef bgil::rgba32f_pixel_t pixel_t;
  typedef bgil::image<pixel_t, false > image_t;

  unsigned const W = 2;

  image_t image(W, 2);

  CHECK_EQUAL( W * sizeof(pixel_t), img::get_scanline_size(image) );

  img::get_pixels_memory(image)[0] = pixel_t(2, 1, 1, 2);
  img::get_pixels_memory(image)[1] = pixel_t(2, 2, 1, 2);
  img::get_pixels_memory(image)[2] = pixel_t(2, 1, 2, 2);
  img::get_pixels_memory(image)[3] = pixel_t(2, 2, 2, 2);

  CHECK_EQUAL( pixel_t(2, 1, 1, 2), bgil::view(image)(0, 0) );
  CHECK_EQUAL( pixel_t(2, 2, 1, 2), bgil::view(image)(1, 0) );
  CHECK_EQUAL( pixel_t(2, 1, 2, 2), bgil::view(image)(0, 1) );
  CHECK_EQUAL( pixel_t(2, 2, 2, 2), bgil::view(image)(1, 1) );
}

TEST_FIXTURE(DefaultValues, GilImageMemoryNoScanlinePadding)
{
  typedef bgil::rgb8_pixel_t pixel_t;
  typedef bgil::image<pixel_t, false > image_t;

  unsigned const W = 3;

  image_t image(W, 2);

  CHECK_EQUAL( 1, sizeof(pixel_t) % 2 );

  CHECK_EQUAL( W * sizeof(pixel_t), img::get_scanline_size(image) );

  img::get_pixels_memory(image)[0] = pixel_t(2, 1, 1);
  img::get_pixels_memory(image)[1] = pixel_t(2, 2, 1);
  img::get_pixels_memory(image)[2] = pixel_t(2, 1, 2);
  img::get_pixels_memory(image)[3] = pixel_t(2, 2, 2);

  CHECK_EQUAL( pixel_t(2, 1, 1), bgil::view(image)(0, 0) );
  CHECK_EQUAL( pixel_t(2, 2, 1), bgil::view(image)(1, 0) );
  CHECK_EQUAL( pixel_t(2, 1, 2), bgil::view(image)(2, 0) );
  CHECK_EQUAL( pixel_t(2, 2, 2), bgil::view(image)(0, 1) );
}

TEST_FIXTURE(DefaultValues, GilRgbToRgba)
{
  // ensure that GIL adds opaque alpha value when default converting from RGB to RGBA

  typedef bgil::rgb8_pixel_t rgb_pixel_t;
  typedef bgil::rgba8_pixel_t rgba_pixel_t;

  typedef bgil::image<rgb_pixel_t, false >  rgb_image_t;
  typedef bgil::image<rgba_pixel_t, false > rgba_image_t;

  rgb_image_t  src_image(1, 1);
  rgba_image_t dst_image(1, 1);

  bgil::view(src_image)(0, 0) = rgb_pixel_t(128, 128, 128);

  bgil::copy_and_convert_pixels( view(src_image), view(dst_image) );

  CHECK_EQUAL( rgba_pixel_t(128, 128, 128, 255), bgil::view(dst_image)(0, 0) );
}

TEST_FIXTURE(DefaultValues, GilRgbaToRgb)
{
  // ensure that GIL premultiplies with alpha when default converting from RGBA to RGB

  typedef bgil::rgb8_pixel_t rgb_pixel_t;
  typedef bgil::rgba8_pixel_t rgba_pixel_t;

  typedef bgil::image<rgb_pixel_t, false >  rgb_image_t;
  typedef bgil::image<rgba_pixel_t, false > rgba_image_t;

  rgba_image_t src_image(1, 1);
  rgb_image_t  dst_image(1, 1);

  bgil::view(src_image)(0, 0) = rgba_pixel_t(128, 128, 128, 128);

  bgil::copy_and_convert_pixels( view(src_image), view(dst_image) );

  CHECK_EQUAL( rgb_pixel_t(64, 64, 64), bgil::view(dst_image)(0, 0) );
}

TEST_FIXTURE(DefaultValues, GilCopyCompatibleImagesWithoutConvert)
{
  typedef bgil::rgba8_pixel_t   pixel_t1;
  typedef bgil::rgb32f_pixel_t  pixel_t2;

  typedef bgil::image<pixel_t1, false > image_t1;
  typedef bgil::image<pixel_t2, false > image_t2;

  image_t1 src_image(1, 1);
  image_t2 dst_image(1, 1);

  bgil::view(src_image)(0, 0) = pixel_t1(2, 2, 2, 2);

  bgil::copy_pixels( view(src_image), view(dst_image) );

  CHECK_EQUAL( pixel_t1(2, 2, 2, 2), bgil::view(src_image)(0, 0) );
  CHECK_EQUAL( pixel_t2(2, 2, 2), bgil::view(dst_image)(0, 0) );
}

//----------------------------------------------------------------------------

int main(int, char const *[])
{
  return UnitTest::RunAllTests();
}
