#define NOMINMAX
#include <windows.h>

#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"

#include <rtgu/image_io/image_io.hpp>
#include <rtgu/image/filter_weight_table.hpp>
#include <rtgu/image/filters.hpp>
#include <rtgu/image/rescale.hpp>
#include <rtgu/image/rescale_any_view.hpp>

#include "test_helpers.hpp"

namespace gil = boost::gil;
namespace imgio = rtgu::image_io;

struct DefaultValues
{
  DefaultValues()
  {
  }

  ~DefaultValues()
  {
  }
};


TEST_FIXTURE(DefaultValues, WeightTable)
{
  gil::bilinear_filter filter;

  gil::detail::weight_table downsample_table;
  downsample_table.reset( filter, 1000, 500 );

  CHECK( downsample_table.window_size() > filter.width() );

  gil::detail::weight_table upsample_table;
  upsample_table.reset( filter, 500, 1000 );

  CHECK( upsample_table.window_size() == 2 * filter.width() + 1 );
}

//----------------------------------------------------------------------------

namespace {

  template <int N>
  struct contrib_mockup
  {
    int left, right;
    float weights[N];

    contrib_mockup() : left(0), right(N-1) {};
  };

};

TEST_FIXTURE(DefaultValues, RescaleLine)
{
  typedef gil::bgra8_pixel_t   src_pixel_t;
  typedef gil::rgba8_pixel_t   dst_pixel_t;
  typedef gil::detail::create_accum_pixel_type<src_pixel_t>::type accum_pixel_t;

  src_pixel_t src[2] = { src_pixel_t(32,64,128,255), src_pixel_t(32,64,128,255) };
  dst_pixel_t dst[1];

  typedef contrib_mockup<2> contrib_mockup_t;
  contrib_mockup_t contrib;
  contrib.weights[0] = 0.5f;
  contrib.weights[1] = 0.5f;

  gil::detail::rescale_line<accum_pixel_t>( src, dst, &contrib, &contrib+1 );

  CHECK_EQUAL( dst_pixel_t(128,64,32,255), dst[0] );
}

TEST_FIXTURE(DefaultValues, RescaleLineDimensionMismatch)
{
  typedef gil::bgra8_pixel_t   src_pixel_t;
  typedef gil::rgba8_pixel_t   dst_pixel_t;

  gil::bilinear_filter filter;

  {
    gil::image<src_pixel_t, false> src(3, 3);
    gil::image<dst_pixel_t, false> dst(7, 7);

    CHECK_THROW( gil::rescale_rows( gil::view(src), gil::view(dst), filter ), std::runtime_error );

    CHECK_THROW( gil::rescale_cols( gil::view(src), gil::view(dst), filter ), std::runtime_error );

    CHECK_NO_THROW( gil::rescale( gil::view(src), gil::view(dst), filter ) );
  }
}

TEST_FIXTURE(DefaultValues, RescaleLineDimensionMatch)
{
  typedef gil::bgra8_pixel_t   src_pixel_t;
  typedef gil::rgba8_pixel_t   dst_pixel_t;

  gil::bilinear_filter filter;

  {
    gil::image<src_pixel_t, false> src(3, 3);
    gil::image<dst_pixel_t, false> dst(3, 7);

    CHECK_NO_THROW( gil::rescale_rows( gil::view(src), gil::view(dst), filter ) );
  }
  {
    gil::image<src_pixel_t, false> src(3, 3);
    gil::image<dst_pixel_t, false> dst(7, 3);

    CHECK_NO_THROW( gil::rescale_cols( gil::view(src), gil::view(dst), filter ) );
  }
}

TEST_FIXTURE(DefaultValues, RescaleImage)
{
  typedef gil::rgba32f_pixel_t pixel_t;
  typedef imgio::image_rgba32f image_t;

  //DebugBreak();

  imgio::any_image result = imgio::read_image("data\\test_fregate.exr");

  imgio::any_image bigger_image = imgio::create_compatible_image( result, result.width() / 4, result.height() / 4 );

  //gil::catmull_rom_filter filter;
  gil::bilinear_filter filter;

  gil::rescale_any_view( view(result), view(bigger_image), filter );

  imgio::write_image("data\\test_bilinear.exr", bigger_image);
}

//----------------------------------------------------------------------------

int main(int, char const *[])
{
  return UnitTest::RunAllTests();
}
