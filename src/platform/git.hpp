#ifndef VOKE_PLATFORM_GIT_HPP
#define VOKE_PLATFORM_GIT_HPP

#include <string>

#include "resource/resource.hpp"

namespace voke {
  const static std::string vokegpu_repository_voke_repositories_url {"https://github.com/vokegpu/voke-repositories/"};
}

namespace voke::platform {
  voke::flags_t sync_git_repository(
    std::string url,
    std::string path_to_clone,
    std::string clone_args = ""
  );
}

#endif
