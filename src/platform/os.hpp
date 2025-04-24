#ifndef VOKE_PLATFORM_OS_HPP
#define VOKE_PLATFORM_OS_HPP

#include <string>

namespace voke::platform {
  #if defined(_WIN32) || defined(_WIN64)
    // add windows path
  #elif defined(__linux__)
    const static std::string voke_system_path {"/opt/vokegpu/voke"};
    const static std::string voke_system_installed_compilers_path {"/opt/vokegpu/voke/installed-compilers.voke"};
  #endif
}

#endif
