#ifndef VOKE_RESOURCE_LIBRARY_HPP
#define VOKE_RESOURCE_LIBRARY_HPP

#include "io/memory.hpp"
#include "argument/argument.hpp"

#include <vector>
#include <string>

namespace voke {
  struct library_t {
  public:
    std::string voke_tag {};
    std::string voke_path {};
    std::string tag {};
    std::string build_system {};
    std::vector<std::string> targets {};
    std::string url {};
    std::string git_clone_args {};
    std::string version {};
    std::string repository_cache_path {};
    std::vector<std::string> include_dirs {};
    std::vector<std::string> binary_win_32_dirs {};
    std::vector<std::string> binary_win_64_dirs {};
    std::vector<std::string> binary_linux_32_dirs {};
    std::vector<std::string> binary_linux_64_dirs {};
    std::string run {};
    std::vector<std::string> libs {};
    std::vector<std::string> includes {};
    std::string compiler {};
    std::string type {};
    std::string arch {};
    uint16_t error_counter {}; 
  };
}

namespace voke::resource {
  voke::flags_t serialize_library_from_argument(
    voke::argument_t &argument,
    voke::library_t &library
  );

  voke::flags_t serialize_library_from_argument_list(
    std::vector<voke::argument_t> &argument_list,
    voke::library_t &library
  );
}

#endif
