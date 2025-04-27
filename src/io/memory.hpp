#ifndef VOKE_IO_MEMORY_HPP
#define VOKE_IO_MEMORY_HPP

#include <string>
#include <cstdint>
#include <vector>

namespace voke {
  typedef uint64_t flags_t;
  typedef uint64_t id_t;
  typedef int32_t shell_result_t;

  enum result {
    SUCCESS                  = 0,
    SUCCESS_PASS             = 1,
    WARNING_INVALID_COMPILER = 2,
    WARNING_INVALID_LIBRARY  = 3,
    ERROR_FAILED             = 4,
    ERROR_TIMEOUT            = 5,
    ERROR_UNKNOWN            = 6
  };
}

namespace voke::io {
  std::vector<std::string> split(
    std::string &str,
    std::string delimiter
  );
}

#endif
