#ifndef VOKE_PLATFORM_GIT_HPP
#define VOKE_PLATFORM_GIT_HPP

#include <string>

namespace voke::platform {
  const static std::string vokegpu_voke_libraries_repository_url {"https://github.com/vokegpu/voke-libraries"};

  void voke_system_git_sync();
}

#endif
