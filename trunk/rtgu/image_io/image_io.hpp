#ifndef RTGU_IMAGE_IO_IMAGE_IO_HPP
#define RTGU_IMAGE_IO_IMAGE_IO_HPP

#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>

#include "gil/borrowed_image.hpp"

#define RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(pixel_type_id) \
  typedef gil::borrowed_image<gil::pixel_type_id##_pixel_t> image_##pixel_type_id;

namespace rtgu { namespace image_io {

  namespace gil = boost::gil;
  namespace mpl = boost::mpl;

  //

  RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(bgra8);
  RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(bgr8);
  RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(gray8);

  RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(rgba16);
  RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(rgb16);
  RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(gray16);

  RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(rgba32f);
  RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(rgb32f);
  RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(gray32f);

  //

  typedef mpl::vector6<
      image_bgr8
    , image_bgra8
    , image_rgb16
    , image_rgba16
    , image_rgb32f
    , image_rgba32f
    >
    all_image_types_t;

  typedef gil::any_image<all_image_types_t> any_image;

  //

  class io_exception : public std::runtime_error
  {
  public:
     explicit io_exception(const std::string& what) : std::runtime_error(what) {}
     virtual ~io_exception() throw() {}
  };

  class unsupported_format_exception : public std::runtime_error
  {
  public:
     explicit unsupported_format_exception(const std::string& what) : std::runtime_error(what) {}
     virtual ~unsupported_format_exception() throw() {}
  };

  //

  /// throws io_exception, unsupported_format_exception
  any_image read_image(char const* filename);

  /// throws io_exception, unsupported_format_exception
  inline void write_image(any_image const& image)
  {
    throw unsupported_format_exception("Not implemented");
  }

  /// throws io_exception, unsupported_format_exception
  template <class View> void write_view(View const& view)
  {
    throw unsupported_format_exception("Not implemented");
  }

} }

#undef RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE

#endif
