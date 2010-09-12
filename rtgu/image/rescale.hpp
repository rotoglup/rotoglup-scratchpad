#ifndef RTGU_IMAGE_RESCALE_HPP
#define RTGU_IMAGE_RESCALE_HPP

#include <stdexcept>
#include <boost/gil/extension/numeric/pixel_numeric_operations.hpp>
#include <boost/gil/extension/numeric/algorithm.hpp>

#ifdef USE_INTEL_TBB
# include <tbb/blocked_range.h>
# include <tbb/parallel_for.h>
#endif // ifdef USE_INTEL_TBB

#include "filter_kernel_1d.hpp"

#ifdef _MSC_VER
// Turn on fast floating-point optimizations
// in 'precise' float_control mode, the 'accumulate_pixels' functions passes plenty of time rounding floats to actual 32 bits.
#pragma float_control( push )
#pragma fp_contract( on )
#pragma fenv_access( off )
#pragma float_control( except, off )
#pragma float_control( precise, off )
#endif

//----------------------------------------------------------------------------

namespace boost { namespace gil {

  // horizontal rescale only, rescale src view into dst view, matching dst view width
  template <typename SrcView, typename DstView, typename Filter>
  inline void rescale_cols(const SrcView& src, const DstView& dst, const Filter& filter);

  // vertical rescale only, rescale src view into dst view, matching dst view height
  template <typename SrcView, typename DstView, typename Filter>
  inline void rescale_rows(const SrcView& src, const DstView& dst, const Filter& filter);

  // rescale src view into dst view, matching dst view dimensions
  template <typename SrcView, typename DstView>
  inline void rescale(const SrcView& src, const DstView& dst
    , const filter_kernel_1d& horizontal_weights
    , const filter_kernel_1d& vertical_weights
    );

  // rescale src view into dst view, matching dst view dimensions
  template <typename SrcView, typename DstView, typename Filter>
  inline void rescale(const SrcView& src, const DstView& dst, const Filter& filter);

} }

//----------------------------------------------------------------------------

namespace boost { namespace gil {

  namespace detail {

    //----------------------------------------------------------------------------

    template <typename SrcChannel, typename DstChannel>
    struct round_channel_fn
    {
      SrcChannel operator()(SrcChannel x) { return x; }
    };

    template <> struct round_channel_fn<float, char> { float operator()(float x) { return (x+0.5f); } };
    template <> struct round_channel_fn<float, short> { float operator()(float x) { return (x+0.5f); } };
    template <> struct round_channel_fn<float, int> { float operator()(float x) { return (x+0.5f); } };
    template <> struct round_channel_fn<float, unsigned char> { float operator()(float x) { return (x+0.5f); } };
    template <> struct round_channel_fn<float, unsigned short> { float operator()(float x) { return (x+0.5f); } };
    template <> struct round_channel_fn<float, unsigned int> { float operator()(float x) { return (x+0.5f); } };

    template <> struct round_channel_fn<double, char> { double operator()(double x) { return (x+0.5); } };
    template <> struct round_channel_fn<double, short> { double operator()(double x) { return (x+0.5); } };
    template <> struct round_channel_fn<double, int> { double operator()(double x) { return (x+0.5); } };
    template <> struct round_channel_fn<double, unsigned char> { double operator()(double x) { return (x+0.5); } };
    template <> struct round_channel_fn<double, unsigned short> { double operator()(double x) { return (x+0.5); } };
    template <> struct round_channel_fn<double, unsigned int> { double operator()(double x) { return (x+0.5); } };

    struct clamp_channel_fn
    {
        template <typename SrcChannel, typename DstChannel>
        void operator()(const SrcChannel& src, DstChannel& dst) {
            typedef typename channel_traits<DstChannel>::value_type dst_value_t;
            typedef typename channel_traits<SrcChannel>::value_type src_value_t;
            dst = dst_value_t(
                    std::min(
                        src_value_t(channel_traits<DstChannel>::max_value())
                      , std::max(
                          src_value_t(channel_traits<DstChannel>::min_value())
                        , round_channel_fn<SrcChannel, DstChannel>()(src)
                        )
                      )
                    );
        }
    };

    template <typename SrcPixel, typename DstPixel>
    void copy_clamp_pixel(const SrcPixel& src, DstPixel& dst) {
        static_for_each(src,dst,clamp_channel_fn());
    }

    //----------------------------------------------------------------------------

    template <typename PixelType> struct create_accum_pixel_type : public create_accum_pixel_type<PixelType> {};

    template <typename Layout>
    struct create_accum_pixel_type<pixel<bits8,Layout> >
    {
      typedef pixel<float, Layout> type;
    };

    template <typename Layout>
    struct create_accum_pixel_type<pixel<bits16,Layout> >
    {
      typedef pixel<float, Layout> type;
    };

    template <typename Layout>
    struct create_accum_pixel_type<pixel<bits32,Layout> >
    {
      typedef pixel<double, Layout> type;
    };

    template <typename Layout>
    struct create_accum_pixel_type<pixel<bits8s,Layout> >
    {
      typedef pixel<float, Layout> type;
    };

    template <typename Layout>
    struct create_accum_pixel_type<pixel<bits16s,Layout> >
    {
      typedef pixel<float, Layout> type;
    };

    template <typename Layout>
    struct create_accum_pixel_type<pixel<bits32s,Layout> >
    {
      typedef pixel<double, Layout> type;
    };

    template <typename Layout>
    struct create_accum_pixel_type<pixel<bits32f,Layout> >
    {
      typedef pixel<float, Layout> type;
    };

    template<class _InIt1, class _InIt2, class _Ty, class _Fn21, class _Fn22>
    inline _Ty accumulate_pixels(_InIt1 _First1, _InIt1 _Last1, _InIt2 _First2, _Ty _Val, _Fn21 _Func1, _Fn22 _Func2)
    {	// return inner product of sequences, using _Func1 and _Func2
      _Ty result(_Val);
      for (; _First1 != _Last1; ++_First1, ++_First2)
        result = _Func1(result, _Func2(*_First1, *_First2));
      return result;
    }

    //----------------------------------------------------------------------------

    /// \brief 1D un-guarded correlation with a variable-size kernel
    /// This function is much like 'correlate_pixels_n' in boost/gil/extension/numeric/algorithm.hpp
    /// except it clamps the result to min/max channel values (because of filters potential clipping artifacts
    template <typename AccumulatorPixelType, typename SrcIterator, typename DstIterator, typename ContribIterator>
    void rescale_line(SrcIterator const src_begin, DstIterator const dst_begin, ContribIterator const contrib_begin, ContribIterator const contrib_end)
    {
      typedef typename pixel_proxy<typename std::iterator_traits<SrcIterator>::value_type>::type src_pixel_t;

      typedef pixel_plus_t<AccumulatorPixelType, AccumulatorPixelType, AccumulatorPixelType> pixel_adder_t;
      typedef pixel_multiplies_scalar_t<src_pixel_t, float, AccumulatorPixelType> pixel_mul_t;                  // NB, no color conversion occuring

      AccumulatorPixelType zero_pixel;
      zero_channels(zero_pixel);

      ContribIterator contrib_it = contrib_begin;
      DstIterator dst_it = dst_begin;

      // iterate over contributions, one contribution for every destination pixel
      while (contrib_it != contrib_end)
      {
        // iterator range on source pixels contributing to current destination pixel
        SrcIterator const src_pixels_begin = src_begin + contrib_it->left;
        SrcIterator const src_pixels_end   = src_begin + (contrib_it->right+1);

        // for every weight: accum += src_pixel * weight
        AccumulatorPixelType accum = accumulate_pixels(
            src_pixels_begin, src_pixels_end
          , contrib_it->weights
          , zero_pixel
          , pixel_adder_t()
          , pixel_mul_t()
          );

        copy_clamp_pixel(accum, *dst_it);

        ++contrib_it;
        ++dst_it;
      }
    }

  }

  //----------------------------------------------------------------------------

} }

//----------------------------------------------------------------------------

template <typename SrcView, typename DstView, typename Filter>
inline void boost::gil::rescale_cols(const SrcView& src, const DstView& dst, const Filter& filter)
{
  typedef typename SrcView::value_type src_pixel_t;
  typedef typename gil::detail::create_accum_pixel_type<src_pixel_t>::type accum_pixel_t;

  if (src.height() != dst.height())
  {
    throw std::runtime_error("rescale_cols: height of source and destination views must match");
  }

  filter_kernel_1d kernel;
  kernel.reset(filter, src.width(), dst.width());

  filter_kernel_1d::const_iterator const kernel_begin = kernel.begin();
  filter_kernel_1d::const_iterator const kernel_end   = kernel.end();

  typedef typename DstView::coord_t coord_t;
  coord_t const height = dst.height();

  for (coord_t y=0; y<height; y++)
  {
    detail::rescale_line<accum_pixel_t>(
        src.row_begin(y)
      , dst.row_begin(y)
      , kernel_begin
      , kernel_end
      );
  }
}

//----------------------------------------------------------------------------

template <typename SrcView, typename DstView, typename Filter>
inline void boost::gil::rescale_rows(const SrcView& src, const DstView& dst, const Filter& filter)
{
  typedef typename SrcView::value_type src_pixel_t;
  typedef typename gil::detail::create_accum_pixel_type<src_pixel_t>::type accum_pixel_t;

  if (src.width() != dst.width())
  {
    throw std::runtime_error("rescale_rows: width of source and destination views must match");
  }

  filter_kernel_1d kernel;
  kernel.reset(filter, src.height(), dst.height());

  filter_kernel_1d::const_iterator const kernel_begin = kernel.begin();
  filter_kernel_1d::const_iterator const kernel_end   = kernel.end();

  typedef typename DstView::coord_t coord_t;
  coord_t const width = dst.width();

  for (coord_t x=0; x<width; x++)
  {
    detail::rescale_line<accum_pixel_t>(
        src.col_begin(x)
      , dst.col_begin(x)
      , kernel_begin
      , kernel_end
      );
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#if USE_INTEL_TBB

namespace {

  using namespace boost;
  using namespace boost::gil;

  template <typename SrcView, typename DstView>
  struct rescale_fn
  {
    const SrcView& src;
    const DstView& dst;
    const gil::detail::weight_table& horizontal_weights;
    const gil::detail::weight_table& vertical_weights;

    rescale_fn(const SrcView& src_, const DstView& dst_, const gil::detail::weight_table& horizontal_weights_, const gil::detail::weight_table& vertical_weights_)
      : src(src_), dst(dst_), horizontal_weights(horizontal_weights_), vertical_weights(vertical_weights_)
    {}

    void operator()( const tbb::blocked_range<int>& range ) const
    {
      int const y_begin = range.begin();
      int const y_end = range.end();

      typedef typename SrcView::value_type src_pixel_t;
      typedef typename gil::detail::create_accum_pixel_type<src_pixel_t>::type accum_pixel_t;

      // allocate intermediary pixels row

      std::vector<accum_pixel_t> intermediate_row( src.width() );

      typedef typename DstView::coord_t coord_t;
      coord_t const src_width  = src.width();
      coord_t const dst_height = dst.height();

      for (coord_t y=y_begin; y<y_end; y++)
      {
        // create the intermediate row by sampling the source image columns

        gil::detail::weight_table::const_iterator const vtable_begin = vertical_weights.begin() + y;
        gil::detail::weight_table::const_iterator const vtable_end   = vtable_begin + 1;

        for (coord_t x=0; x<src_width; x++)
        {
          gil::detail::rescale_line<accum_pixel_t>(
              src.col_begin(x)
            , intermediate_row.begin() + x
            , vtable_begin
            , vtable_end
            );
        }

        // scale horizontally the intermediate row into the destination row

        gil::detail::weight_table::const_iterator const htable_begin = horizontal_weights.begin();
        gil::detail::weight_table::const_iterator const htable_end   = horizontal_weights.end();

        gil::detail::rescale_line<accum_pixel_t>(
            intermediate_row.begin()
          , dst.row_begin(y)
          , htable_begin
          , htable_end
          );
      }
    }
  };

}

#endif

//----------------------------------------------------------------------------

template <typename SrcView, typename DstView>
inline void boost::gil::rescale(const SrcView& src, const DstView& dst, const filter_kernel_1d& horizontal_weights, const filter_kernel_1d& vertical_weights)
{
#if USE_INTEL_TBB

  typedef typename SrcView::value_type src_pixel_t;
  typedef typename gil::detail::create_accum_pixel_type<src_pixel_t>::type accum_pixel_t;

  // allocate intermediary pixels row

  std::vector<accum_pixel_t> intermediate_row( src.width() );

  typedef typename DstView::coord_t coord_t;
  coord_t const src_width  = src.width();
  coord_t const dst_height = dst.height();

  const int IdealGrainSize = 1;

  tbb::parallel_for( tbb::blocked_range<int>(0,dst_height,IdealGrainSize), rescale_fn<SrcView,DstView>(src, dst, horizontal_weights, vertical_weights) );

#else

  typedef typename SrcView::value_type src_pixel_t;
  typedef typename gil::detail::create_accum_pixel_type<src_pixel_t>::type accum_pixel_t;

  // allocate intermediary pixels row

  std::vector<accum_pixel_t> intermediate_row( src.width() );

  typedef typename DstView::coord_t coord_t;
  coord_t const src_width  = src.width();
  coord_t const dst_height = dst.height();

  for (coord_t y=0; y<dst_height; y++)
  {
    // create the intermediate row by sampling the source image columns

    filter_kernel_1d::const_iterator const vtable_begin = vertical_weights.begin() + y;
    filter_kernel_1d::const_iterator const vtable_end   = vtable_begin + 1;

    for (coord_t x=0; x<src_width; x++)
    {
      detail::rescale_line<accum_pixel_t>(
          src.col_begin(x)
        , intermediate_row.begin() + x
        , vtable_begin
        , vtable_end
        );
    }

    // scale horizontally the intermediate row into the destination row

    filter_kernel_1d::const_iterator const htable_begin = horizontal_weights.begin();
    filter_kernel_1d::const_iterator const htable_end   = horizontal_weights.end();

    detail::rescale_line<accum_pixel_t>(
        intermediate_row.begin()
      , dst.row_begin(y)
      , htable_begin
      , htable_end
      );
  }

#endif
}

//----------------------------------------------------------------------------

template <typename SrcView, typename DstView, typename Filter>
inline void boost::gil::rescale(const SrcView& src, const DstView& dst, const Filter& filter)
{
  // construct weights tables

  filter_kernel_1d horizontal_weights;
  horizontal_weights.reset(filter, src.width(), dst.width());

  filter_kernel_1d vertical_weights;
  vertical_weights.reset(filter, src.height(), dst.height());

  // rescale

  rescale( src, dst, horizontal_weights, vertical_weights );
}

//----------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma float_control( push )
#endif

#endif
