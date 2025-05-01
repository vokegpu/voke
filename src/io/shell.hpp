#ifndef VOKE_IO_SHELL_HPP
#define VOKE_IO_SHELL_HPP

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

#endif
