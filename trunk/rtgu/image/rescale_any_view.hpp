#ifndef RTGU_IMAGE_RESCALE_ANY_VIEW_HPP
#define RTGU_IMAGE_RESCALE_ANY_VIEW_HPP

#include <boost/gil/extension/dynamic_image/any_image_view.hpp>
#include "rescale.hpp"

//----------------------------------------------------------------------------

namespace boost { namespace gil {

  template <typename ViewTypes, typename Filter>
  inline void rescale_any_view(any_image_view<ViewTypes>& src, any_image_view<ViewTypes>& dst, const Filter& filter);

  template <typename ViewTypes>
  inline void rescale_any_view(any_image_view<ViewTypes>& src, any_image_view<ViewTypes>& dst
    , const filter_kernel_1d& horizontal_weights, const filter_kernel_1d& vertical_weights
    );

} }

//----------------------------------------------------------------------------

namespace boost { namespace gil { 

namespace detail {

  struct rescale_obj : public binary_operation_obj<rescale_obj>
  {
    const filter_kernel_1d& _horizontal_weights;
    const filter_kernel_1d& _vertical_weights;

    rescale_obj(const filter_kernel_1d& horizontal_weights, const filter_kernel_1d& vertical_weights)
      : _horizontal_weights(horizontal_weights), _vertical_weights(vertical_weights)
    {}

    template <typename V1, typename V2>
    GIL_FORCEINLINE bool apply_compatible(const V1& v1, const V2& v2) const
    {
      return rescale( v1, v2, _horizontal_weights, _vertical_weights );
    }
  };
}

} }

template <typename ViewTypes, typename Filter>
inline void boost::gil::rescale_any_view(any_image_view<ViewTypes>& src, any_image_view<ViewTypes>& dst, const Filter& filter)
{
  // construct kernels
  
  filter_kernel_1d horizontal_weights;
  horizontal_weights.reset(filter, src.width(), dst.width());

  filter_kernel_1d vertical_weights;
  vertical_weights.reset(filter, src.height(), dst.height());

  // rescale

  apply_operation( src, dst, detail::rescale_obj(horizontal_weights, vertical_weights) );
}

template <typename ViewTypes>
inline void boost::gil::rescale_any_view(any_image_view<ViewTypes>& src, any_image_view<ViewTypes>& dst, const filter_kernel_1d& horizontal_weights, const filter_kernel_1d& vertical_weights)
{
  apply_operation( src, dst, detail::rescale_obj(horizontal_weights, vertical_weights) );
}

//----------------------------------------------------------------------------

#endif
