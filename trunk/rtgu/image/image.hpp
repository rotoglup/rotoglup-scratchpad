#ifndef RTGU_IMAGE_IMAGE_HPP
#define RTGU_IMAGE_IMAGE_HPP

#include <boost/gil/pixel.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/extension/dynamic_image/any_image.hpp>
#include <boost/gil/extension/dynamic_image/apply_operation.hpp>

#include "detail/image_allocator.hpp"

namespace rtgu { namespace image {

  namespace gil = boost::gil;
  namespace mpl = boost::mpl;

  //

  typedef gil::image<gil::bgra8_pixel_t, false, detail::byte_allocator> image_bgra8;
  typedef gil::image<gil::bgr8_pixel_t,  false, detail::byte_allocator> image_bgr8;
  typedef gil::image<gil::gray8_pixel_t, false, detail::byte_allocator> image_grey8;

  typedef gil::image<gil::rgba32f_pixel_t, false, detail::byte_allocator> image_rgba32f;
  typedef gil::image<gil::rgb32f_pixel_t,  false, detail::byte_allocator> image_rgb32f;
  typedef gil::image<gil::gray32f_pixel_t, false, detail::byte_allocator> image_grey32f;

  //

  typedef mpl::vector3<
      image_grey8
    , image_bgr8
    , image_bgra8
    >
    all_image_types_t;

  typedef gil::any_image<all_image_types_t> any_image;

  //

  //! Returns the non-constant-pixel view of an image (just to be in rtgu::image namespace)
  /*template <typename Pixel, bool IsPlanar, typename Alloc> inline 
  const typename gil::image<Pixel,IsPlanar,Alloc>::view_t& view(gil::image<Pixel,IsPlanar,Alloc>& img) { return img._view; }
  */

  //

  inline void create_compatible_image(any_image const& base_image, int width, int height, any_image& result);

  //

  template <typename Pixel>
  Pixel* get_pixels_memory(gil::image<Pixel, false>& image)
  {
    // TODO replace by interleaved_view_get_raw_data ?
    return reinterpret_cast<Pixel*>( gil::interleaved_view_get_raw_data(gil::view(image)) ); // &( gil::view(image)(0,0) );
  }

  template <typename Pixel>
  std::ptrdiff_t get_scanline_size(gil::image<Pixel, false> const& image)
  {
    // TODO consider gil image alignment - possible ?
    return image.width() * sizeof(Pixel);
  }

} }

// TODO
// insert_image
// crop
// accum_pixels
// premultiply_alpha
// rgb_to_rgbmax
// _conversions_
// VFlip

//----------------------------------------------------------------------------
// implementation

namespace rtgu { namespace image {

  namespace detail {

    struct any_image_create_compatible
    {
      typedef void result_type;

      int width_, height_;
      any_image& result_;

      any_image_create_compatible(int width, int height, any_image& result) : width_(width), height_(height), result_(result) {}

      template <typename Image> result_type operator()(Image const& img) const { result_ = any_image(Image()); result_.recreate(width_, height_); }
    };
  
  }

  void create_compatible_image(any_image const& base_image, int width, int height, any_image& result)
  {
    return apply_operation(base_image, detail::any_image_create_compatible(width, height, result));
  }

} }

#endif
