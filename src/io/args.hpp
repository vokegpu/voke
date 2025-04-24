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

  void fill(int32_t args_size, char **pp_args);
  void push_back_arg_if_necessary(voke::io::argument_t &arg);
  
  voke::flags_t find_argument_by_prefix(
    std::vector<std::string> prefix_list,
    voke::io::argument_t &argument
  );
}

#endif
