#ifndef BOOST_GIL_FILTER_WEIGHT_TABLE_HPP
#define BOOST_GIL_FILTER_WEIGHT_TABLE_HPP

#include <vector>
#include <math.h>
#include <boost/utility.hpp>

// adapted from FreeImage source code (see FreeImage/Source/FreeImageToolkit/Resize.h)

namespace boost { namespace gil {

namespace detail
{
  //----------------------------------------------------------------------------
  /**
    Filter weights table.<br>
    This class stores contribution information for an entire line (row or column).
  */
  class weight_table : boost::noncopyable
  {
    /** 
      Sampled filter weight table.<br>
      Contribution information for a single pixel
    */
    struct Contribution {
      /// Normalized weights of neighboring pixels (not owned, pointer to elements of _weights_storage)
      float* weights;
	    /// Bounds of source pixels window
	    int left, right;   
    };

	  /// Row (or column) of contribution weights 
    std::vector<Contribution> _contribution_table;      // TODO add allocator template parameter or replace std::vector use entirely
    std::vector<float> _weights_storage;

	  /// Filter window size (of affecting source pixels) 
	  unsigned _window_size;

  public:

    typedef std::vector<Contribution>::const_iterator const_iterator;

    weight_table() {};

	  /** 
	  Allocate and compute the weights table
	  @param filter Filter used for upsampling or downsampling
	  @param src_size Length (in pixels) of the source line
	  @param dst_size Length (in pixels) of the destination line
	  */
    template <typename Filter>
    void reset(Filter const& filter, unsigned src_size, unsigned dst_size);

    unsigned window_size() const { return _window_size; };

    const_iterator begin() const { return _contribution_table.begin(); };
    const_iterator end() const   { return _contribution_table.end(); };
  };

  //----------------------------------------------------------------------------

} // namespace detail

} }   // namespace boost::gil

template <typename Filter>
void boost::gil::detail::weight_table::reset(Filter const& filter, unsigned uSrcSize, unsigned uDstSize)
{
  float const dFilterWidth = static_cast<float>( filter.width() );

  // scale factor from source to destination
  float const dScale = static_cast<float>(uDstSize) / static_cast<float>(uSrcSize);

  float dWidth, dFScale;

  if (dScale < 1.0)
  {
    // minification
    dWidth  = dFilterWidth / dScale; 
    dFScale = dScale; 
  }
  else
  {
    // magnification
    dWidth  = dFilterWidth;
    dFScale = 1.0f;
  }

  // window size is the number of sampled source pixels per destination pixel
  _window_size = 2 * static_cast<int>( ceil(dWidth) ) + 1; 

  // allocate list of contributions 
  _contribution_table.resize( uDstSize );

  // allocate vector for every needed weight factor
  _weights_storage.resize( uDstSize * _window_size );

  // offset for discrete to continuous coordinate conversion
  float dOffset = (0.5f / dScale) - 0.5f;

  // scan through line of contributions

  for (unsigned u = 0; u < uDstSize; u++)
  {
    float dCenter = static_cast<float>(u) / dScale + dOffset;   // reverse mapping to source coordinates

    // find the significant edge points that affect the pixel

    int iLeft  = static_cast<int>( floor(dCenter - dWidth) );
    int iRight = static_cast<int>(  ceil(dCenter + dWidth) );

    // clamp edge points to borders
    // FIXME could handle wrapping of image here

    iLeft  = std::max( iLeft, 0 );
    iRight = std::min( iRight, static_cast<int>(uSrcSize) - 1 );
    
    // cut edge points to fit in filter window in case of spill-off due to rounding

    if ((iRight - iLeft + 1) > static_cast<int>(_window_size))
    {
      if (iLeft < (int(uSrcSize) - 1 / 2)) // FIXME missing () ?
      {    
        iLeft++;
      }
      else
      {
        iRight--; 
      }
    }

    // calculate weights

    float* const weight_base_pointer = &_weights_storage[ u * _window_size ];
    float* weight_storage = weight_base_pointer;

    float total_weight = 0.f;  // zero sum of weights

    for (int srcx = iLeft; srcx <= iRight; srcx++)
    {
      float x = dFScale * (dCenter - static_cast<float>(srcx));
      float filter_value = filter.filter(x);
      float weight = dFScale * filter_value;
      
      total_weight += weight;

      *weight_storage++ = weight;
    }
    
    // normalize weight of neighbouring points

    if ((total_weight > 0) && (total_weight != 1))
    {
      float* weight_storage = weight_base_pointer;

      for (int iSrc = iLeft; iSrc <= iRight; iSrc++)
      {
        *weight_storage++ /= total_weight; 
      }
      
      // simplify the filter, discarding null weights at the right

      int weight_index = iRight - iLeft;
      while (weight_index > 0 && weight_base_pointer[weight_index] == 0.f)
      {
        iRight--;
        weight_index--;
      }
    }

    // store contribution for current pixel

    _contribution_table[u].left    = iLeft; 
    _contribution_table[u].right   = iRight;
    _contribution_table[u].weights = weight_base_pointer;
  } 
}

#endif
