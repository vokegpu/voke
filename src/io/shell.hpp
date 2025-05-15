#ifndef VOKE_IO_SHELL_HPP
#define VOKE_IO_SHELL_HPP

#include "memory.hpp"

#include <sstream>
#include <string>

namespace voke {
  enum verbose_level {
    LEVEL_ONE,
    LEVEL_TWO
  };

  class shell {
  public:
    static voke::verbose_level verbose_level;
    static voke::shell_result_t result;
  protected:
    std::ostringstream raw {};
    bool already_sent {};
  protected:
    void send() {
      voke::shell_result_t result {};
      switch (voke::shell::verbose_level) {
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
      this->already_sent = true;
    }
  public:
    ~shell() {
      if (!this->already_sent) {
        this->send();
      }
    }

    template<typename t>
    shell &operator << (t content) {
      this->raw << content;
      return *this;
    }
  public:
    template<typename t>
    bool operator == (t result) {
      if (!this->already_sent) {
        this->send();
      }

      return voke::shell::result == result;
    }

    template<typename t>
    bool operator != (t result) {
      if (!this->already_sent) {
        this->send();
      }

      return voke::shell::result != result;
    }
  };
}

#endif
