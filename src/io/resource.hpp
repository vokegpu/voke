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
    std::string tag {};
    std::string url {};
    std::string git_clone_args {};
    std::string cmake_dir {};
    std::string generator {};
    std::string version {};
    std::vector<std::string> include_dirs {};
    std::vector<std::string> binary_win_32_dirs {};
    std::vector<std::string> binary_win_64_dirs {};
    std::vector<std::string> binary_linux_32_dirs {};
    std::vector<std::string> binary_linux_64_dirs {};
    std::vector<std::string> libs {};
    std::vector<std::string> includes {};
    std::string compiler {};
    std::string type {};
    std::string arch {};
  };

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
  };

  voke::flags_t extract_installed_compiler_info_from_line(
    std::string &line,
    voke::io::compiler_t &compiler
  );

  voke::flags_t extract_installed_library_info_from_line(
    std::string &line,
    voke::io::library_t &library
  );

  voke::flags_t extract_library_info_from_line(
    std::string &line,
    voke::io::library_t &library
  );
}

#endif
