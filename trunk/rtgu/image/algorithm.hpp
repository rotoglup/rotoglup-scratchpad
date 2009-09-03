#ifndef RTGU_IMAGE_ALGORITHM_HPP
#define RTGU_IMAGE_ALGORITHM_HPP

#include "image.hpp"

//----------------------------------------------------------------------------

namespace rtgu { namespace image {

  inline void premultiply_alpha(image_bgra8::view_t const& image);
  inline void premultiply_alpha(image_rgba32f::view_t const& image);

  template <typename View>
  inline void vertical_flip(View& view);

} }

//----------------------------------------------------------------------------

namespace rtgu { namespace image {

  namespace detail
  {
    template <typename view_t, typename rgba_pixel_t, typename rgb_pixel_t>
    void premultiply_alpha_impl(view_t const& view)
    {
      gil::gil_function_requires< gil::MutableImageViewConcept<view_t> >();

      typedef typename view_t::iterator iterator;
      iterator it = view.begin();
      iterator const end = view.end();

      while (it != end)
      {
        rgb_pixel_t  temp;
        rgba_pixel_t src( *it );
        rgba_pixel_t dst;

        boost::gil::default_color_converter()(src, temp);
        boost::gil::default_color_converter()(temp, dst);
        boost::gil::get_color(dst, boost::gil::alpha_t()) = boost::gil::get_color(src, boost::gil::alpha_t());

        *it = dst;
        ++it;
      }
    }
  }

} }

//----------------------------------------------------------------------------

inline void rtgu::image::premultiply_alpha(image_bgra8::view_t const& view)
{
  detail::premultiply_alpha_impl<image_bgra8::view_t, boost::gil::bgra8_pixel_t, boost::gil::bgr8_pixel_t>(view);
}

inline void rtgu::image::premultiply_alpha(image_rgba32f::view_t const& view)
{
  detail::premultiply_alpha_impl<image_rgba32f::view_t, boost::gil::rgba32f_pixel_t, boost::gil::rgb32f_pixel_t>(view);
}

//----------------------------------------------------------------------------

template <typename View>
inline void rtgu::image::vertical_flip(View& view)
{
  gil::gil_function_requires< gil::MutableImageViewConcept<View> >();

  typedef typename View::coord_t coord_t;

  coord_t y_start = 0;
  coord_t y_end   = view.height() - 1;
  while (y_start < y_end)
  {
    std::swap_ranges( view.row_begin(y_start), view.row_end(y_start), view.row_begin(y_end) );

    ++y_start;
    --y_end;
  }
}

//----------------------------------------------------------------------------

// TODO
// insert_image
// crop
// accum_pixels
// rgb_to_rgbmax
// _conversions_

#endif
