#ifndef VOKE_IO_MEMORY_HPP
#define VOKE_IO_MEMORY_HPP

#include <iostream>

namespace voke {
  typedef uint64_t flags_t;

  enum result {
    SUCCESS = 0,
    PASS    = 1,
    FAILED  = 2,
    TIMEOUT = 3,
    UNKNOWN = 4
  };
}

#endif