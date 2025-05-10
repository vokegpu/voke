#ifndef VOKE_IO_MEMORY_HPP
#define VOKE_IO_MEMORY_HPP

#include <string>
#include <cstdint>
#include <vector>

namespace voke {
  typedef uint32_t flags_t;
  typedef uint32_t id_t;
  typedef int32_t shell_result_t;

  constexpr static size_t any {666};
  constexpr static size_t empty {0};
  constexpr static size_t not_empty {720};

  constexpr static bool mandatory {true};
  constexpr static bool optional {false};

  enum result : uint8_t {
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

  void replace(
    std::string &str_to_replace,
    std::string to_replace,
    std::string replace
  );

  template<typename t>
  constexpr bool has(voke::flags_t bits, t bit) {
    return (bits & bit) == bit;
  }
}

#endif
