#ifndef VOKE_CMD_HELP_HPP
#define VOKE_CMD_HELP_HPP

#include "voke/io/memory.hpp"
#include <vector>
#include <string>

namespace voke::cmd::help {
  voke::flags_t assert();
  voke::flags_t run();
}

#endif
