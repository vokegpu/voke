#ifndef VOKE_CMD_SYNC_HPP
#define VOKE_CMD_SYNC_HPP

#include "io/memory.hpp"

#include <vector>
#include <string>

namespace voke::cmd::sync {
  voke::flags_t assert();
  voke::flags_t run();
}

#endif
