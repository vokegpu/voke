#ifndef VOKE_IO_ARGS_HPP
#define VOKE_IO_ARGS_HPP

#include "memory.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

namespace voke {
  struct argument_t {
  public:
    std::string prefix {};
    std::vector<std::string> values {};
    std::string raw {};
    size_t line {};
  };

  typedef std::vector<argument_t> argument_list_t;
  typedef std::vector<std::string> prefix_list_t;
}

namespace voke::io {
  std::vector<voke::argument_t> args_find_all(
    const std::vector<std::string> &prefixes
  );

  voke::flags_t args_contains_any_non(
    const std::vector<std::string> &prefixes
  );

  voke::flags_t args_contains_any(
    const std::vector<std::string> &prefixes
  );

  void extract(
    int32_t args_size,
    char **pp_args,
    std::vector<std::string> &in_args
  );

  void fill(
    std::vector<std::string> &in_args,
    std::vector<voke::argument_t> &voke_args,
    bool should_serialize_quote = false
  );

  void push_back_arg_if_necessary(
    std::vector<voke::argument_t> &args,
    voke::argument_t &arg
  );

  voke::flags_t throw_unknown_command_or_arguments();
}

#endif
