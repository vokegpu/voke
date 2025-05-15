#ifndef VOKE_IO_SHELL_HPP
#define VOKE_IO_SHELL_HPP

#include "memory.hpp"
#include "verbose_level.hpp"

#include <sstream>
#include <string>

namespace voke {
  class shell {
  public:
    static voke::shell_result_t result;
  protected:
    std::ostringstream raw {};
    bool already_sent {};
    bool should_expose_output_forced {};
  protected:
    void send() {
      if (
        voke::io_verbose_level < static_cast<uint8_t>(voke::verbose_level::LEVEL_THREE)
        &&
        !this->should_expose_output_forced
      ) {
        #if defined(_WIN32)
          this->raw << " >nul 2>&1";
        #elif defined(__linux__)
          this->raw << " >/dev/null 2>&1";
        #endif
      }

      voke::shell::result = std::system(this->raw.str().c_str());
      this->already_sent = true;
    }
  public:
    shell(bool should_expose_output_forced = false) {
      this->should_expose_output_forced = should_expose_output_forced;
    }

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
