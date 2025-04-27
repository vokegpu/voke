#ifndef VOKE_PLATFORM_OS_HPP
#define VOKE_PLATFORM_OS_HPP

#include "io/memory.hpp"
#include "io/resource.hpp"
#include "voke.hpp"

#include <sstream>
#include <string>

namespace voke {
  class shell {
  public:
    static voke::shell_result_t result;
  protected:
    std::ostringstream raw {};
  public:
    ~shell() {
      voke::shell_result_t result {};
      switch (voke::app.verbose_level) {
      case voke::verbose_level::LEVEL_ONE:
        #if defined(_WIN32)
          this->raw << " >nul 2>&1";
        #elif defined(__linux__)
          this->raw << " >/dev/null 2>&1";
        #endif
        break;
      case voke::verbose_level::LEVEL_TWO:
        break;
      }

      voke::shell::result = std::system(this->raw.str().c_str());
    }

    template<typename t>
    shell &operator << (t content) {
      this->raw << content;
      return *this;
    }
  };
}

namespace voke::platform {
  #if defined(_WIN32) || defined(_WIN64)
    // add windows path
  #elif defined(__linux__)
    const static std::string voke_system_path {"/opt/vokegpu/voke"};
    const static std::string voke_system_installed_compilers_path {"/opt/vokegpu/voke/installed-compilers.voke"};
    const static std::string voke_system_installed_libraries_path {"/opt/vokegpu/voke/installed-libraries.voke"};
    const static std::string voke_system_repository_cache_path {"/opt/vokegpu/voke/.repository"};
  #endif

  voke::flags_t voke_system_fetch_installed_compilers();
  voke::flags_t voke_system_fetch_installed_libraries();
  voke::flags_t voke_system_fetch_library(voke::io::library_t &library);
  voke::flags_t voke_system_lookup_compilers_from_host_library(voke::io::library_t &library, std::vector<std::string> &compilers);
  voke::flags_t voke_system_fetch_compilers_info_from_host_library(voke::io::library_t &library, std::vector<std::string> &compilers);
}

#endif
