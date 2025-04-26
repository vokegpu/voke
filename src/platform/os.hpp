#ifndef VOKE_PLATFORM_OS_HPP
#define VOKE_PLATFORM_OS_HPP

#include "io/memory.hpp"
#include "io/resource.hpp"
#include <string>

namespace voke::platform {
  #if defined(_WIN32) || defined(_WIN64)
    // add windows path
  #elif defined(__linux__)
    const static std::string voke_system_path {"/opt/vokegpu/voke"};
    const static std::string voke_system_installed_compilers_path {"/opt/vokegpu/voke/installed-compilers.voke"};
    const static std::string voke_system_installed_libraries_path {"/opt/vokegpu/voke/installed-libraries.voke"};
  #endif

  voke::flags_t voke_system_fetch_compilers();
  voke::flags_t voke_system_fetch_libraries();
  voke::flags_t voke_system_fetch_library(voke::io::library_t &library);
  voke::flags_t voke_system_lookup_compilers_from_library(voke::io::library_t &library, std::vector<std::string> &compilers);
}

#endif
