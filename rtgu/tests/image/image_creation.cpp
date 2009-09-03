#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <rtgu/image/image.hpp>

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


TEST_FIXTURE(DefaultValues, EmptyConstructor)
{
  img::image_bgra8 image(256, 256);
  
  CHECK_EQUAL( image.width(),  256 );
  CHECK_EQUAL( image.height(), 256 );
}

namespace {

  struct check_identical_pixel_types_op
  {
    typedef bool result_type;

    template <typename Image1, typename Image2> result_type operator()(Image1 const& img1, Image2 const& img2) { return false; };
    template <typename Pixel, bool IsPlanar1, bool IsPlanar2, typename Alloc1, typename Alloc2> result_type operator()(boost::gil::image<Pixel, IsPlanar1, Alloc1> const& img1, boost::gil::image<Pixel, IsPlanar2, Alloc2> const& img2) { return true; };
  };

  //! returns true if and only if the 2 given dynamic images have the same pixel type
  bool check_identical_pixel_types(img::any_image const& img1, img::any_image const& img2)
  {
    return apply_operation( img1, img2, check_identical_pixel_types_op() );
  }

};

TEST_FIXTURE(DefaultValues, CreateCompatibleImage)
{
  img::image_bgra8 real_source_image(256, 256);

  img::any_image source_image( real_source_image );

  img::any_image compatible_image = img::create_compatible_image( source_image, 128, 128 );
  
  CHECK( check_identical_pixel_types(source_image, compatible_image) );

  CHECK_EQUAL( 128, compatible_image.width() );
  CHECK_EQUAL( 128, compatible_image.height() );
}

int main(int, char const *[])
{
  return UnitTest::RunAllTests();
}
