#ifndef VOKE_PLATFORM_GIT_HPP
#define VOKE_PLATFORM_GIT_HPP

#include <string>

#include "voke/resource/resource.hpp"

namespace voke {
  const static std::string vokegpu_repository_voke_repositories_url {"https://github.com/vokegpu/voke-repositories/"};

  struct platform_git_sync_repository_info {
  public:
    std::string url {};
    std::string path {};
    std::string clone_args {};
    bool enable_safe_directory {};
  };
}

namespace voke::platform {
  voke::flags_t sync_git_repository(
    voke::platform_git_sync_repository_info &sync_repository_info
  );
}

#endif
