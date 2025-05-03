#ifndef VOKE_PLATFORM_OS_HPP
#define VOKE_PLATFORM_OS_HPP

#include "resource/compiler.hpp"
#include "resource/library.hpp"
#include "io/memory.hpp"

#include <sstream>
#include <string>

namespace voke::platform {
  #if defined(_WIN32) || defined(_WIN64)
    // add windows path
  #elif defined(__linux__)
    const static std::string voke_system_path {"/opt/vokegpu/voke"};
    const static std::string voke_system_installed_compilers_path {"/opt/vokegpu/voke/installed-compilers.voke"};
    const static std::string voke_system_installed_libraries_path {"/opt/vokegpu/voke/installed-libraries.voke"};
    const static std::string voke_system_repository_cache_dir {"/opt/vokegpu/voke/.repository/"};
  #endif

  voke::flags_t voke_system_fetch_installed_compilers();
  voke::flags_t voke_system_fetch_installed_libraries();
  voke::flags_t voke_system_fetch_sync_library_target(voke::library_t &library);
  voke::flags_t voke_system_lookup_compilers_from_host_library(voke::library_t &library, std::vector<std::string> &compilers);
  voke::flags_t voke_system_fetch_compilers_info_from_host_library(voke::library_t &library, std::vector<std::string> &compilers);
  voke::flags_t compile_libraries();
}

#endif
