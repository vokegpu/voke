#ifndef VOKE_IO_VERBOSE_LEVEL_HPP
#define VOKE_IO_VERBOSE_LEVEL_HPP

#include <cstdint>

namespace voke {
  enum class verbose_level : uint8_t {
    LEVEL_ONE,
    LEVEL_TWO,
    LEVEL_THREE
  };

  extern uint8_t io_verbose_level;
}

#endif
