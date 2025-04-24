#ifndef VOKE_IO_LOG_HPP
#define VOKE_IO_LOG_HPP

#include <iostream>
#include <cstdint>
#include <sstream>

namespace voke {
  class log {
  public:
    static std::ostringstream buffer;
    static bool buffered;
    static bool tracked;
    static int32_t status;
  public:
    static int32_t flush() {
      if (voke::log::buffered) {
        #if defined(__ANDROID__)
          __android_log_print(ANDROID_LOG_VERBOSE, "EKG", "%s", voke::log::buffer.str().c_str());
        #else
          std::cout << voke::log::buffer.str();
        #endif

        voke::log::buffer = std::ostringstream {};
        voke::log::buffered = false;
      }

      return voke::log::status;
    }

    static void print(std::string_view msg) {
      std::cout << msg << std::endl;
    }

    template<typename t>
    static void trace(bool should, t trace, bool interrupt_runtime = false) {
      if (!should) {
        return;
      }

      std::cout << trace << std::endl;
      if (interrupt_runtime) {
        std::terminate();
      }
    }
  public:
    template<typename t>
    explicit log(t content) {
      std::cout << content << std::endl;
    }

    explicit log() {
      voke::log::buffered = true;
    }

    ~log() {
      voke::log::buffer << '\n';

      #if defined(EKG_LOG_DEBUG)
        voke::log::flush();
        std::cout << std::endl;
      #endif
    }

    template<typename t>
    log &operator<<(const t &value) {
      voke::log::buffer << value;
      return *this;
    }
  };
}

#endif
