#ifndef VOKE_IO_MEMORY_HPP
#define VOKE_IO_MEMORY_HPP

#include <string>
#include <cstdint>
#include <vector>

namespace voke {
  typedef uint64_t flags_t;
  typedef uint64_t id_t;

  enum result {
    SUCCESS = 0,
    PASS    = 1,
    FAILED  = 2,
    TIMEOUT = 3,
    UNKNOWN = 4
  };
}

namespace voke::io {
  std::vector<std::string> split(std::string &str, char delimiter);
}

#endif
