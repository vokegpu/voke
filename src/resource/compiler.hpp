#ifndef VOKE_RESOURCE_COMPILER_HPP
#define VOKE_RESOURCE_COMPILER_HPP

#include "io/memory.hpp"
#include "argument/argument.hpp"

#include <string>
#include <fstream>

namespace voke {
  struct compiler_t {
  public:
    std::string voke_tag {};
    std::string voke_path {};
    std::string tag {};
    std::string binary_dir {};
    std::string lib_dir {};
    std::string include_dir {};
    std::string c_compiler {};
    std::string cpp_compiler {};
    std::string version {};
    bool is_installed {};
    uint16_t error_counter {}; 
  };
}

namespace voke::resource {
  voke::flags_t serialize_compiler_from_argument(
    voke::argument_t &argument,
    voke::compiler_t &compiler
  );

  voke::flags_t serialize_compiler_from_argument_list(
    std::vector<voke::argument_t> &argument_list,
    voke::compiler_t &compiler
  );
}

#endif
