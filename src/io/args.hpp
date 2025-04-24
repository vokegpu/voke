#ifndef VOKE_IO_ARGS_HPP
#define VOKE_IO_ARGS_HPP

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include "memory.hpp"

namespace voke::io {
  struct argument_t {
  public:
    std::string prefix {};
    std::vector<std::string> values {};
    std::string raw {};
  };

  std::vector<voke::io::argument_t> args_find_all(
    std::vector<std::string> prefixes
  );

  voke::flags_t args_contains_any_non(
    std::vector<std::string> prefixes
  );

  voke::flags_t args_contains_any(
    std::vector<std::string> prefixes
  );

  void fill(
    int32_t args_size,
    char **pp_args,
    std::vector<voke::io::argument_t> &args
  );

  void push_back_arg_if_necessary(
    std::vector<voke::io::argument_t> &args,
    voke::io::argument_t &arg
  );
}

#endif
