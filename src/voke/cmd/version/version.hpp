#ifndef VOKE_CMD_VERSION_HPP
#define VOKE_CMD_VERSION_HPP

#include "voke/io/memory.hpp"
#include <vector>
#include <string>

namespace voke::cmd::version {
  voke::flags_t assert();
  voke::flags_t run();
}

#endif
