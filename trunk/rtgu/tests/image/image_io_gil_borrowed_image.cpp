#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <boost/gil/gil_all.hpp>

#include <rtgu/image/image.hpp>
#include <rtgu/image_io/gil/borrowed_image.hpp>

#include "test_helpers.hpp"

namespace gil = boost::gil;
namespace img = rtgu::image;

//----------------------------------------------------------------------------

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

  struct test_deleter
  {
    bool& _deleted;

    test_deleter(bool& deleted) : _deleted(deleted) {}
    void operator()(void* pointer) { _deleted = true; };
  };

}

TEST_FIXTURE(DefaultValues, GilExtBorrowedImage)
{
  typedef boost::gil::bgra8_pixel_t pixel_t;
  typedef boost::gil::borrowed_image<pixel_t> image_t;

  bool image_has_been_deleted = false;

  test_deleter image_deleter(image_has_been_deleted);

  {
    boost::shared_ptr<image_t> image_holder;

    {
      void* image_pointer = NULL;

      boost::shared_ptr<void> image_memory(image_pointer, image_deleter);

      image_holder.reset( new image_t(128, 128, NULL, 128*sizeof(pixel_t), false, image_memory) );

      image_memory.reset();
    }

    // here 'image_holder' should have a reference on 'image_memory'

    CHECK_EQUAL( false, image_has_been_deleted );
  }

  // here 'image_holder' should have released the reference on 'image_memory'

  CHECK_EQUAL( true, image_has_been_deleted );
}

TEST_FIXTURE(DefaultValues, GilExtBorrowedImageHasView)
{
  typedef boost::gil::bgra8_pixel_t pixel_t;
  typedef boost::gil::borrowed_image<pixel_t> image_t;

  pixel_t const A(128, 128, 128, 128);
  pixel_t const B(255, 255, 128, 128);

  pixel_t pixels[2];

  image_t image(1, 2, pixels, 1*sizeof(pixel_t), false, boost::shared_ptr<void>());

  view(image)(0,0) = A;
  view(image)(0,1) = B;

  CHECK_EQUAL( A, view(image)(0,0) );
  CHECK_EQUAL( B, view(image)(0,1) );

  CHECK_EQUAL( A, pixels[0] );
  CHECK_EQUAL( B, pixels[1] );
}

TEST_FIXTURE(DefaultValues, GilExtBorrowedImageHasView_UpsideDown)
{
  typedef boost::gil::bgra8_pixel_t pixel_t;
  typedef boost::gil::borrowed_image<pixel_t> image_t;

  pixel_t const A(128, 128, 128, 128);
  pixel_t const B(255, 255, 128, 128);

  pixel_t pixels[2];

  image_t image(1, 2, pixels, 1*sizeof(pixel_t), true, boost::shared_ptr<void>());

  view(image)(0,0) = A;
  view(image)(0,1) = B;

  CHECK_EQUAL( A, view(image)(0,0) );
  CHECK_EQUAL( B, view(image)(0,1) );

  CHECK_EQUAL( B, pixels[0] );
  CHECK_EQUAL( A, pixels[1] );
}

//----------------------------------------------------------------------------

int main(int, char const *[])
{
  return UnitTest::RunAllTests();
}
