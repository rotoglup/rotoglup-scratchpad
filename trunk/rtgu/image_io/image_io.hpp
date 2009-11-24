#ifndef RTGU_IMAGE_IO_IMAGE_IO_HPP
#define RTGU_IMAGE_IO_IMAGE_IO_HPP

#include <stdexcept>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>

#include <FreeImage/FreeImage.h>    // TODO remove from this header, used only for 'create_image'

#include "gil/borrowed_image.hpp"

#define RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE(pixel_type_id) \
  typedef gil::borrowed_image<gil::pixel_type_id##_pixel_t> image_##pixel_type_id

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

  template <typename PixelType>
  void create_image(gil::borrowed_image<PixelType>& image, int width, int height);

  inline any_image create_compatible_image(any_image const& image, int width, int height);

  //

  /// throws io_exception, unsupported_format_exception
  any_image read_image(char const* filename);

  /// throws io_exception
  inline void write_image(char const* filename, any_image const& image);
  // TODO add a write_image that takes a templated image

  /// throws io_exception, unsupported_format_exception
  template <class View> void write_view(View const& view)
  {
    throw unsupported_format_exception("Not implemented");
  }

} }

#undef RTGU_IMAGE_IO_DECLARE_IMAGE_TYPE

//----------------------------------------------------------------------------

namespace rtgu { namespace image_io { namespace detail {

  template <typename PixelType>
  struct pixel_descriptor_t {};

#define RTGU_IMAGE_IO_PIXEL_DESCRIPTOR(pixel_type_id, fi_image_type, fi_pixel_bpp)  \
  template <> struct pixel_descriptor_t<gil::pixel_type_id##_pixel_t>  \
  { \
    static const int image_type = (fi_image_type); \
    static const int bit_count = (fi_pixel_bpp);  \
  };

  RTGU_IMAGE_IO_PIXEL_DESCRIPTOR(bgra8, FIT_BITMAP, 32);
  RTGU_IMAGE_IO_PIXEL_DESCRIPTOR(bgr8,  FIT_BITMAP, 24);
  RTGU_IMAGE_IO_PIXEL_DESCRIPTOR(gray8, FIT_BITMAP,  8);

  RTGU_IMAGE_IO_PIXEL_DESCRIPTOR(rgba16, FIT_RGBA16, 16*4);
  RTGU_IMAGE_IO_PIXEL_DESCRIPTOR(rgb16,  FIT_RGB16,  16*3);
  RTGU_IMAGE_IO_PIXEL_DESCRIPTOR(gray16, FIT_UINT16, 16);

  RTGU_IMAGE_IO_PIXEL_DESCRIPTOR(rgba32f, FIT_RGBAF, 32*4);
  RTGU_IMAGE_IO_PIXEL_DESCRIPTOR(rgb32f,  FIT_RGBF,  32*3);
  RTGU_IMAGE_IO_PIXEL_DESCRIPTOR(gray32f, FIT_FLOAT, 32);

#undef RTGU_IMAGE_IO_PIXEL_DESCRIPTOR

} } }

template <typename PixelType>
void rtgu::image_io::create_image(gil::borrowed_image<PixelType>& image, int width, int height)
{
  typedef detail::pixel_descriptor_t<PixelType> pixel_descriptor;

  FIBITMAP* fi_image = FreeImage_AllocateT( static_cast<FREE_IMAGE_TYPE>(pixel_descriptor::image_type), width, height, pixel_descriptor::bit_count);

  if (fi_image == NULL)
  {
    throw std::bad_alloc();
  }

  boost::shared_ptr<void> fi_image_holder( fi_image, FreeImage_Unload );

  unsigned const pitch = FreeImage_GetPitch( fi_image );
  BYTE* const pixels_memory = FreeImage_GetBits( fi_image );
  PixelType* pixels = reinterpret_cast<PixelType*>( pixels_memory );

  image.reconstruct( width, height, pixels, pitch, true, fi_image_holder );
}

namespace rtgu { namespace image_io { namespace detail {

  struct create_compatible_image_op
  {
    typedef any_image result_type;

    int width, height;

    create_compatible_image_op(int width_, int height_) : width(width_), height(height_) {}

    template <typename PixelType>
    result_type operator()(gil::borrowed_image<PixelType> const& img)
    {
      gil::borrowed_image<PixelType> image;
      create_image( image, width, height );
      return any_image( image );
    }
  };

} } }

rtgu::image_io::any_image rtgu::image_io::create_compatible_image(any_image const& image, int width, int height)
{
  return apply_operation( image, detail::create_compatible_image_op(width, height) );
}

//----------------------------------------------------------------------------

namespace rtgu { namespace image_io { namespace detail {

  struct get_FIBITMAP_op
  {
    typedef FIBITMAP* result_type;

    template <typename PixelType>
    result_type operator()(gil::borrowed_image<PixelType> const& img)
    { return reinterpret_cast<FIBITMAP*>( img._memory_holder.get() ); }
  };

  inline FIBITMAP* get_FIBITMAP(any_image const& image)
  {
    return apply_operation( image, get_FIBITMAP_op() );
  }

} } }

void rtgu::image_io::write_image(char const* filename, any_image const& image)
{
  FREE_IMAGE_FORMAT fi_image_format = FreeImage_GetFIFFromFilename(filename);

  FIBITMAP* fi_image = detail::get_FIBITMAP(image);

  BOOL result = FreeImage_Save(fi_image_format, fi_image, filename);
}

//----------------------------------------------------------------------------

#endif
