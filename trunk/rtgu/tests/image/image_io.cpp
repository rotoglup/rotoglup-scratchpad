#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <rtgu/image_io/image_io.hpp>

#include "test_helpers.hpp"

namespace gil   = boost::gil;
namespace imgio = rtgu::image_io;

//----------------------------------------------------------------------------

gil::bgr8_pixel_t const bgr8_RED(0, 0, 255);
gil::bgr8_pixel_t const bgr8_GREEN(0, 255, 0);
gil::bgr8_pixel_t const bgr8_BLUE(255, 0, 0);
gil::bgr8_pixel_t const bgr8_WHITE(255, 255, 255);

gil::rgb16_pixel_t const rgb16_RED(0xFFFF, 0, 0);
gil::rgb16_pixel_t const rgb16_GREEN(0, 0xFFFF, 0);
gil::rgb16_pixel_t const rgb16_BLUE(0, 0, 0xFFFF);
gil::rgb16_pixel_t const rgb16_WHITE(0xFFFF, 0xFFFF, 0xFFFF);

gil::rgb32f_pixel_t const rgb32f_hdr_RED(1.5f, 0, 0);
gil::rgb32f_pixel_t const rgb32f_hdr_GREEN(0, 1.5f, 0);
gil::rgb32f_pixel_t const rgb32f_hdr_BLUE(0, 0, 1.5f);
gil::rgb32f_pixel_t const rgb32f_hdr_WHITE(1.5f, 1.5f, 1.5f);

struct DefaultValues
{
  DefaultValues()
  {
  }

  ~DefaultValues()
  {
  }
};

namespace {

  template <typename Pixel>
  struct has_pixel_type_op
  {
    typedef bool result_type;

    template <typename Image> result_type operator()(Image const& img)   { return false; };
    result_type operator()(gil::borrowed_image<Pixel> const& img) { return true; };
  };

  template <typename Pixel>
  bool has_pixel_type(imgio::any_image const& img)
  {
    return apply_operation( img, has_pixel_type_op<Pixel>() );
  }

}

TEST_FIXTURE(DefaultValues, ReadImage_RGB8)
{
  typedef gil::bgr8_pixel_t pixel_t;
  typedef imgio::image_bgr8 image_t;

  imgio::any_image result = imgio::read_image("data\\tiny_rgb8.png");

  CHECK_EQUAL( 2, result.width() );
  CHECK_EQUAL( 2, result.height() );

  CHECK( has_pixel_type<pixel_t>(result) );

  image_t::view_t concrete_view = gil::view( result._dynamic_cast<image_t>() );

  CHECK_EQUAL( bgr8_BLUE,  concrete_view(0,0) );
  CHECK_EQUAL( bgr8_GREEN, concrete_view(1,0) );
  CHECK_EQUAL( bgr8_RED,   concrete_view(0,1) );
  CHECK_EQUAL( bgr8_WHITE, concrete_view(1,1) );
}

TEST_FIXTURE(DefaultValues, ReadImage_RGB16)
{
  typedef gil::rgb16_pixel_t pixel_t;
  typedef imgio::image_rgb16 image_t;

  imgio::any_image result = imgio::read_image("data\\tiny_rgb16.png");

  CHECK_EQUAL( 2, result.width() );
  CHECK_EQUAL( 2, result.height() );

  CHECK( has_pixel_type<pixel_t>(result) );

  image_t::view_t concrete_view = gil::view( result._dynamic_cast<image_t>() );

  CHECK_EQUAL( rgb16_BLUE,  concrete_view(0,0) );
  CHECK_EQUAL( rgb16_GREEN, concrete_view(1,0) );
  CHECK_EQUAL( rgb16_RED,   concrete_view(0,1) );
  CHECK_EQUAL( rgb16_WHITE, concrete_view(1,1) );
}

TEST_FIXTURE(DefaultValues, ReadImage_RGB32F)
{
  typedef gil::rgb32f_pixel_t pixel_t;
  typedef imgio::image_rgb32f image_t;

  imgio::any_image result = imgio::read_image("data\\tiny_rgbf.exr");

  CHECK_EQUAL( 2, result.width() );
  CHECK_EQUAL( 2, result.height() );

  CHECK( has_pixel_type<pixel_t>(result) );

  image_t::view_t concrete_view = gil::view( result._dynamic_cast<image_t>() );

  CHECK_EQUAL( rgb32f_hdr_BLUE,  concrete_view(0,0) );
  CHECK_EQUAL( rgb32f_hdr_GREEN, concrete_view(1,0) );
  CHECK_EQUAL( rgb32f_hdr_RED,   concrete_view(0,1) );
  CHECK_EQUAL( rgb32f_hdr_WHITE, concrete_view(1,1) );
}

//----------------------------------------------------------------------------

#define TEST_IMAGE_CREATION(pixel_type_id)  \
  { \
    typedef gil::pixel_type_id##_pixel_t pixel_t; \
    gil::borrowed_image<pixel_t> image; \
    imgio::create_image(image, 256, 128); \
    CHECK_EQUAL( 256, image.width() );  \
    CHECK_EQUAL( 128, image.height() ); \
  }

TEST_FIXTURE(DefaultValues, CreateImages)
{
  TEST_IMAGE_CREATION( bgra8 );
  TEST_IMAGE_CREATION( bgr8 );
  TEST_IMAGE_CREATION( gray8 );

  TEST_IMAGE_CREATION( rgba16 );
  TEST_IMAGE_CREATION( rgb16 );
  TEST_IMAGE_CREATION( gray16 );

  TEST_IMAGE_CREATION( rgba32f );
  TEST_IMAGE_CREATION( rgb32f );
  TEST_IMAGE_CREATION( gray32f );
}

//----------------------------------------------------------------------------

int main(int, char const *[])
{
  return UnitTest::RunAllTests();
}
