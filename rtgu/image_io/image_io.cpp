#include "image_io.hpp"

#include <FreeImage/FreeImage.h>

namespace gil = boost::gil;

namespace imgio = rtgu::image_io;

imgio::any_image imgio::read_image(char const* filename)
{
  imgio::any_image result;

  FREE_IMAGE_FORMAT const fi_format = FreeImage_GetFileType(filename);

  // read image

  FIBITMAP* fi_image = FreeImage_Load(fi_format, filename);

  if (fi_image == NULL)
    throw io_exception("FreeImage_Load failed");

  boost::shared_ptr<void> fi_image_holder( fi_image, FreeImage_Unload );

  // query FreeImage image information

  FREE_IMAGE_TYPE const image_type = FreeImage_GetImageType( fi_image );

  unsigned const bits_per_pixel = FreeImage_GetBPP( fi_image );
  unsigned const width = FreeImage_GetWidth( fi_image );
  unsigned const height = FreeImage_GetHeight( fi_image );

  unsigned const pitch = FreeImage_GetPitch( fi_image );

  BYTE* const pixels_memory = FreeImage_GetBits( fi_image );

  // transpose the FreeImage memory layout to the corresponding image type

#define RTGU_IMAGE_IO_IMPL_CREATE_RESULT(pixel_type_id) \
  typedef gil::pixel_type_id##_pixel_t  pixel_t;  \
  typedef imgio::image_##pixel_type_id  image_t;  \
  pixel_t* pixels = reinterpret_cast<pixel_t*>( pixels_memory );  \
  result = any_image( image_t( width, height, pixels, pitch, true, fi_image_holder ) );

  if (image_type == FIT_BITMAP)
  {
    if (bits_per_pixel == 32)
    {
      RTGU_IMAGE_IO_IMPL_CREATE_RESULT( bgra8 )
    }
    else if (bits_per_pixel == 24)
    {
      RTGU_IMAGE_IO_IMPL_CREATE_RESULT( bgr8 )
    }
    else if (bits_per_pixel == 8)
    {
      if (FreeImage_GetColorType( fi_image ) == FIC_MINISBLACK)
      {
        RTGU_IMAGE_IO_IMPL_CREATE_RESULT( gray8 )
      }
      else
      {
        throw unsupported_format_exception("no support for paletted images other than greyscale");
      }
    }
    else
    {
      // no support for other pixel sizes (e.g. 16 bit 565 images)
      throw unsupported_format_exception("unsupported FIT_BITMAP image type");
    }
  }
  else if (image_type == FIT_UINT16)
  {
    RTGU_IMAGE_IO_IMPL_CREATE_RESULT( gray16 )
  }
  else if (image_type == FIT_FLOAT)
  {
    RTGU_IMAGE_IO_IMPL_CREATE_RESULT( gray32f )
  }
  else if (image_type == FIT_RGB16)
  {
    RTGU_IMAGE_IO_IMPL_CREATE_RESULT( rgb16 )
  }
  else if (image_type == FIT_RGBA16)
  {
    RTGU_IMAGE_IO_IMPL_CREATE_RESULT( rgba16 )
  }
  else if (image_type == FIT_RGBF)
  {
    RTGU_IMAGE_IO_IMPL_CREATE_RESULT( rgb32f )
  }
  else if (image_type == FIT_RGBAF)
  {
    RTGU_IMAGE_IO_IMPL_CREATE_RESULT( rgba32f )
  }
  else
  {
    // missing:
    // FIT_INT16
    // FIT_UINT32
    // FIT_INT32
    // FIT_DOUBLE
    // FIT_COMPLEX
    throw unsupported_format_exception("unsupported image type");
  }

  return result;
}

