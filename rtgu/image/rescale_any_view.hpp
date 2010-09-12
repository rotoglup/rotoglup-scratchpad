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
    , const detail::weight_table& horizontal_weights, const detail::weight_table& vertical_weights
    );

} }

//----------------------------------------------------------------------------

namespace boost { namespace gil { namespace detail
{
  template <typename Views>
  struct rescale_obj
  {
    typedef void result_type;        // required typedef

    any_image_view<Views>& _dst_view;
    const detail::weight_table& _horizontal_weights;
    const detail::weight_table& _vertical_weights;

    rescale_obj(any_image_view<Views>& dst_view, const detail::weight_table& horizontal_weights, const detail::weight_table& vertical_weights)
      : _dst_view(dst_view), _horizontal_weights(horizontal_weights), _vertical_weights(vertical_weights)
    {}

    template <typename SrcView>
    void operator()(SrcView& src) const
    {
      return rescale( src, _dst_view._dynamic_cast<SrcView>(), _horizontal_weights, _vertical_weights );
    }
  };
} } }

template <typename ViewTypes, typename Filter>
inline void boost::gil::rescale_any_view(any_image_view<ViewTypes>& src, any_image_view<ViewTypes>& dst, const Filter& filter)
{
  // construct weights tables

  detail::weight_table horizontal_weights;
  horizontal_weights.reset(filter, src.width(), dst.width());

  detail::weight_table vertical_weights;
  vertical_weights.reset(filter, src.height(), dst.height());

  // rescale

  apply_operation( src, detail::rescale_obj<ViewTypes>(dst, horizontal_weights, vertical_weights) );
}

template <typename ViewTypes>
inline void boost::gil::rescale_any_view(any_image_view<ViewTypes>& src, any_image_view<ViewTypes>& dst, const detail::weight_table& horizontal_weights, const detail::weight_table& vertical_weights)
{
  apply_operation( src, detail::rescale_obj<ViewTypes>(dst, horizontal_weights, vertical_weights) );
}

//----------------------------------------------------------------------------

#endif
