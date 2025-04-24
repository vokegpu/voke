#ifndef VOKE_IO_RESOURCE_HPP
#define VOKE_IO_RESOURCE_HPP

#include "memory.hpp"

#include <string>
#include <fstream>

namespace voke::io {
  struct library_t {
  public:
    std::string voke_tag {};
    std::string voke_path {};
  };

  struct compiler_t {
  public:
    std::string voke_tag {};
    std::string voke_path {};
    std::string binary_dir {};
    bool is_installed {};
  };

  voke::flags_t extract_compiler_info_from_line(
    std::string &line,
    voke::io::compiler_t &compiler
  );
}

#endif
