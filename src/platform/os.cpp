#include "os.hpp"
#include "io/resource.hpp"
#include "voke.hpp"
#include "io/log.hpp"

#include <iostream>

voke::flags_t voke::platform::voke_system_fetch_compilers() {
  voke::flags_t result {voke::result::SUCCESS};

  std::ifstream installed_compilers(voke::platform::voke_system_installed_compilers_path);
  if (installed_compilers.is_open()) {
    std::string line {};
    while (getline(installed_compilers, line)) {
      if (line.empty()) {
        continue;
      }

      voke::io::compiler_t compiler {};
      if (
        voke::io::extract_installed_compiler_info_from_line(
          line,
          compiler
        ) != voke::result::SUCCESS
      ) {
        voke::log() << "error: invalid 'installed-compilers.voke' syntax at line:\n" << line;
        result = voke::result::WARNING_INVALID_COMPILER;
        continue;
      }

      voke::app.compilers.push_back(compiler);
      voke::log() << "detail: " << compiler.tag << " version " << compiler.version;
    }
  }

  return result;
}

voke::flags_t voke::platform::voke_system_fetch_libraries() {
  voke::flags_t result {voke::result::SUCCESS};

  std::ifstream installed_libraries(voke::platform::voke_system_installed_libraries_path);
  if (installed_libraries.is_open()) {
    std::string line {};
    while (getline(installed_libraries, line)) {
      if (line.empty()) {
        continue;
      }

      voke::io::library_t library {};
      if (
        voke::io::extract_installed_library_info_from_line(
          line,
          library
        ) != voke::result::SUCCESS
      ) {
        voke::log() << "error: invalid 'installed-libraries.voke' syntax at line:\n" << line;
        result = voke::result::WARNING_INVALID_LIBRARY;
        continue;
      }

      voke::app.libraries.push_back(library);
      voke::log() << "detail: " << library.tag << " version " << library.version;
    }
  }

  return result;
}

voke::flags_t voke::platform::voke_system_compile_library(
  voke::io::library_t &library
) {
  return voke::result::SUCCESS;
}
