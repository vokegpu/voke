#ifndef VOKE_CMD_SYNC_HPP
#define VOKE_CMD_SYNC_HPP

#include "io/memory.hpp"

#include <vector>
#include <string>

namespace voke {
  enum cmd_sync_mode : uint8_t {
    SYNC_LIBRARY,
    SYNC_TARGET,
    SYNC_ALL_LIBRARIES,
    SYNC_ALL_TARGETS
  };
}

namespace voke::cmd::sync {
  extern voke::cmd_sync_mode mode;

  voke::flags_t assert();
  voke::flags_t run();
}

#endif
