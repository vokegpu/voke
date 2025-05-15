#ifndef VOKE_IO_LOG_HPP
#define VOKE_IO_LOG_HPP

#include "verbose_level.hpp"

#include <iostream>
#include <cstdint>
#include <sstream>

#define VOKE_LOG_DEBUG

namespace voke {
  class log {
  protected:
    uint8_t verbose_level_necessary {};
  public:
    static std::ostringstream buffer;
    static bool buffered;
    static std::string error;
    static int32_t status;
    static bool debug;
    static bool two_verbose_level_required;
  public:
    static int32_t flush() {
      if (voke::log::buffered) {
        #if defined(__ANDROID__)
          __android_log_print(ANDROID_LOG_VERBOSE, "VOKE", "%s", voke::log::buffer.str().c_str());
        #else
          std::cout << voke::log::buffer.str();
        #endif

        voke::log::buffer = std::ostringstream {};
        voke::log::buffered = false;
      }

      return voke::log::status;
    }
  public:
    explicit log(
      voke::verbose_level verbose_level_necessary = voke::verbose_level::LEVEL_ONE
    ) {
      voke::log::buffered = true;
      this->verbose_level_necessary = static_cast<uint8_t>(verbose_level_necessary);
    }

    ~log() {
      if (voke::io_verbose_level < this->verbose_level_necessary) {
        return;
      }

      voke::log::buffer << '\n';

      if (voke::log::debug) {
        std::cout << voke::log::buffer.str();
        voke::log::buffer = {};
      }
    }

    template<typename t>
    log &operator<<(t value) {
      if (voke::io_verbose_level < this->verbose_level_necessary) {
        return *this;
      }

      voke::log::buffer << value;
      return *this;
    }
  };
}

#define VOKE_ASSERT(status, message, returned) if ((status) != 0) { message; return returned; }

#endif
