#ifndef VOKE_PLATFORM_OS_HPP
#define VOKE_PLATFORM_OS_HPP

#include "resource/resource.hpp"
#include "io/memory.hpp"

#include <sstream>
#include <string>

namespace voke {
  #if defined(_WIN32) || defined(_WIN64)
    // add windows path
  #elif defined(__linux__)
    const static std::string system_dir_path {"/opt/vokegpu/voke"};
    const static std::string system_installed_compilers_path {"/opt/vokegpu/voke/installed-compilers.voke"};
    const static std::string system_installed_libraries_path {"/opt/vokegpu/voke/installed-libraries.voke"};
    const static std::string system_cached_repositories_dir_path {"/opt/vokegpu/voke/.repositories/"};
  #endif

  const static std::string local {"local"};
  const static std::string host {"host"};
}

namespace voke::platform {
  voke::flags_t voke_system_fetch_installed_compilers();
  voke::flags_t voke_system_fetch_installed_libraries();

  voke::flags_t voke_system_fetch_library_target(
    voke::library_t &library,
    voke::target_t &target
  );

  voke::flags_t voke_system_fetch_library_target_operations(
    voke::library_t &library,
    voke::target_t &target,
    std::vector<voke::operation_t> &operations
  );

  voke::flags_t voke_system_compile_host_library(
    voke::library_t &library,
    voke::target_t &target,
    std::vector<voke::operation_t> &operations,
    voke::compiler_t &compiler
  );
}

#endif
