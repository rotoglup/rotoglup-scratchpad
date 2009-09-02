/*
    Copyright 2005-2007 Adobe Systems Incorporated
   
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).

    See http://opensource.adobe.com/gil for most recent version including documentation.
*/
/*************************************************************************************************/

#ifndef GIL_BORROWED_IMAGE_H
#define GIL_BORROWED_IMAGE_H

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief Templated borrowed_image, borrowed memory (from boost/gil/borrowed_image.hpp, February 12, 2007 version)
////////////////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <memory>
#include <boost/gil/gil_config.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/gil/metafunctions.hpp>
#include <boost/gil/algorithm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

namespace boost { namespace gil {

////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup ImageModel PixelBasedModel
/// \brief container interface, coupling a view and a memory holder. Models ImageConcept, PixelBasedConcept
/// 
/// A 2D container whose elements are pixels. It is templated over the pixel type, no planar support.
/// This container keeps a reference on the memory allocation used to represent the pixels, along with a view to 
/// access those pixels.
///
/// Note that its element type does not have to be a pixel. \p borrowed_image can be instantiated with any Regular element, 
/// in which case it models the weaker RandomAccess2DImageConcept and does not model PixelBasedConcept
///
////////////////////////////////////////////////////////////////////////////////////////

template <typename Pixel>
class borrowed_image {
public:
    typedef typename boost::shared_ptr<void>         memory_holder_t;     //!< this is ugly, should be done otherwise (not using shared_ptr), too much work for now

    typedef typename view_type_from_pixel<Pixel, false>::type view_t;
    typedef typename view_t::const_t                 const_view_t;
    typedef typename view_t::point_t                 point_t;
    typedef typename view_t::coord_t                 coord_t;
    typedef typename view_t::value_type              value_type;
    typedef coord_t                                  x_coord_t;
    typedef coord_t                                  y_coord_t;

    const point_t&          dimensions()            const { return _view.dimensions(); }
    x_coord_t               width()                 const { return _view.width(); }
    y_coord_t               height()                const { return _view.height(); }

    borrowed_image() {}

    borrowed_image(const point_t& dimensions, Pixel* pixels_base, std::size_t row_size_in_memunits, bool upside_down, const memory_holder_t& memory_holder) {
        initialize_memory(dimensions, pixels_base, row_size_in_memunits, upside_down, memory_holder);
    }
    borrowed_image(x_coord_t width, y_coord_t height, Pixel* pixels_base, std::size_t row_size_in_memunits, bool upside_down, const memory_holder_t& memory_holder) {
        initialize_memory(point_t(width,height), pixels_base, row_size_in_memunits, upside_down, memory_holder);
    }

    ~borrowed_image() {
    }

    void swap(borrowed_image& img) { // required by MutableContainerConcept
        using std::swap;
        swap(_memory_holder,  img._memory_holder);
        swap(_view,           img._view); 
    }    

    view_t       _view;      // contains pointer to the pixels, the borrowed_image size and ways to navigate pixels
private:
    memory_holder_t _memory_holder;

    void initialize_memory(const point_t& dimensions, Pixel* pixels_base, std::size_t row_size_in_memunits, bool upside_down, const boost::shared_ptr<void>& memory_holder) { 
      _memory_holder = memory_holder;
      unsigned char * memory = reinterpret_cast<unsigned char*>( pixels_base );
      if (upside_down)
      {
        _view = gil::flipped_up_down_view( view_t(dimensions, typename view_t::locator( typename view_t::locator(typename view_t::x_iterator(memory), row_size_in_memunits) ) ) );
      }
      else
      {
        _view = view_t(dimensions, typename view_t::locator( typename view_t::x_iterator(memory), row_size_in_memunits ));
      }
    }
};

template <typename Pixel>
void swap(borrowed_image<Pixel>& im1,borrowed_image<Pixel>& im2) {
    im1.swap(im2); 
}

template <typename Pixel1, typename Pixel2>
bool operator==(const borrowed_image<Pixel1>& im1,const borrowed_image<Pixel2>& im2) {
    if ((void*)(&im1)==(void*)(&im2)) return true;
    if (const_view(im1).dimensions()!=const_view(im2).dimensions()) return false;
    return equal_pixels(const_view(im1),const_view(im2));
}
template <typename Pixel1, typename Pixel2>
bool operator!=(const borrowed_image<Pixel1>& im1,const borrowed_image<Pixel2>& im2) {return !(im1==im2);}

///@{
/// \name view, const_view
/// \brief Get an borrowed_image view from an borrowed_image

/// \ingroup ImageModel

/// \brief Returns the non-constant-pixel view of an borrowed_image
template <typename Pixel> inline 
const typename borrowed_image<Pixel>::view_t& view(borrowed_image<Pixel>& img) { return img._view; }

/// \brief Returns the constant-pixel view of an borrowed_image
template <typename Pixel> inline 
const typename borrowed_image<Pixel>::const_view_t const_view(const borrowed_image<Pixel>& img) { 
    return static_cast<const typename borrowed_image<Pixel>::const_view_t>(img._view); 
}
///@}

/////////////////////////////
//  PixelBasedConcept
/////////////////////////////

template <typename Pixel>
struct channel_type<borrowed_image<Pixel> > : public channel_type<Pixel> {}; 

template <typename Pixel>
struct color_space_type<borrowed_image<Pixel> >  : public color_space_type<Pixel> {};

template <typename Pixel>
struct channel_mapping_type<borrowed_image<Pixel> > : public channel_mapping_type<Pixel> {};

template <typename Pixel>
struct is_planar<borrowed_image<Pixel> > : public mpl::bool_<false> {};

//#ifdef _MSC_VER
//#pragma warning(pop)
//#endif

} }  // namespace boost::gil

#endif
