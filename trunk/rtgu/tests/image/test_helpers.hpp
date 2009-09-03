#include <boost/gil/gil_all.hpp>

namespace {

  namespace gil = boost::gil;

  template <class STREAM, typename ChannelValue, typename Layout>
  STREAM& operator<<(STREAM& stream, gil::pixel<ChannelValue, Layout> const& pixel)
  {
    return (stream << "'unknown gil pixel'");
  }

  template <class STREAM>
  STREAM& operator<<(STREAM& stream, gil::rgba8_pixel_t const& pixel)
  {
    return (stream << "gil::rgba8_pixel_t(" << static_cast<int>(gil::get_color(pixel, gil::red_t())) << ", " << static_cast<int>(gil::get_color(pixel, gil::green_t())) << ", " << static_cast<int>(gil::get_color(pixel, gil::blue_t())) << ", " << static_cast<int>(gil::get_color(pixel, gil::alpha_t())) << ")");
  }

  template <class STREAM>
  STREAM& operator<<(STREAM& stream, gil::bgra8_pixel_t const& pixel)
  {
    return (stream << "gil::bgra8_pixel_t(" << static_cast<int>(gil::get_color(pixel, gil::red_t())) << ", " << static_cast<int>(gil::get_color(pixel, gil::green_t())) << ", " << static_cast<int>(gil::get_color(pixel, gil::blue_t())) << ", " << static_cast<int>(gil::get_color(pixel, gil::alpha_t())) << ")");
  }

  template <class STREAM>
  STREAM& operator<<(STREAM& stream, gil::rgb8_pixel_t const& pixel)
  {
    return (stream << "gil::rgb8_pixel_t(" << static_cast<int>(gil::get_color(pixel, gil::red_t())) << ", " << static_cast<int>(gil::get_color(pixel, gil::green_t())) << ", " << static_cast<int>(gil::get_color(pixel, gil::blue_t())) << ")");
  }

  template <class STREAM>
  STREAM& operator<<(STREAM& stream, gil::bgr8_pixel_t const& pixel)
  {
    return (stream << "gil::bgr8_pixel_t(" << static_cast<int>(gil::get_color(pixel, gil::red_t())) << ", " << static_cast<int>(gil::get_color(pixel, gil::green_t())) << ", " << static_cast<int>(gil::get_color(pixel, gil::blue_t())) << ")");
  }

  template <class STREAM>
  STREAM& operator<<(STREAM& stream, gil::rgb32f_pixel_t const& pixel)
  {
    return (stream << "gil::rgb32f_pixel_t(" << gil::get_color(pixel, gil::red_t()) << ", " << gil::get_color(pixel, gil::green_t()) << ", " << gil::get_color(pixel, gil::blue_t()) << ")");
  }

}

#define CHECK_NO_THROW(expression) \
    do \
    { \
        bool caught_ = false; \
        try { expression; } \
        catch (...) { caught_ = true; } \
        if (caught_) \
          UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), "Exception thrown while none expected"); \
    } while(0)
