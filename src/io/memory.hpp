#ifndef VOKE_IO_MEMORY_HPP
#define VOKE_IO_MEMORY_HPP

#include <iostream>

namespace voke {
  typedef uint64_t flags_t;

  enum result {
    SUCCESS = 0,
    TIMEOUT = 1,
    UNKNOWN = 2
  };
}

#endif