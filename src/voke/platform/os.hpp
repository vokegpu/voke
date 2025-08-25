#ifndef VOKE_PLATFORM_OS_HPP
#define VOKE_PLATFORM_OS_HPP

#include "voke/resource/resource.hpp"
#include "voke/io/memory.hpp"

#include <sstream>
#include <string>

namespace voke {
  extern std::string system_dir_path;
  extern std::string system_installed_compilers_path;
  extern std::string system_installed_libraries_path;
  extern std::string system_cached_repositories_dir_path;

  const static std::string local {"local"};
  const static std::string host {"host"};
}

namespace voke::platform {
  voke::flags_t voke_system_init();

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
    voke::compiler_t &compiler,
    std::vector<voke::library_t> &new_installed_local_libraries
  );
}

#endif
