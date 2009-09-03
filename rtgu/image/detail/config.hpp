#ifndef RTGU_IMAGE_DETAIL_CONFIG_HPP
#define RTGU_IMAGE_DETAIL_CONFIG_HPP

#include <limits>
#include <stdlib.h>

namespace rtgu { namespace image { namespace detail {

  void* malloc(std::size_t size)
  {
    return ::malloc(size);
  }

  void free(void* pointer)
  {
    return ::free(pointer);
  }

} } }

#endif