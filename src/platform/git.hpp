#ifndef VOKE_PLATFORM_GIT_HPP
#define VOKE_PLATFORM_GIT_HPP

#include <string>

#include "resource/library.hpp"

namespace voke::platform {
  const static std::string vokegpu_voke_libraries_repository_url {"https://github.com/vokegpu/voke-libraries"};

  voke::flags_t sync_git_repository(
    std::string url,
    std::string path_to_clone,
    std::string clone_args = ""
  );

  void voke_system_fetch_voke_library_git(voke::library_t &library);
}

#endif
