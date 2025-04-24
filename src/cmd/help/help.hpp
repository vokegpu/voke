#ifndef VOKE_CMD_HELP_HPP
#define VOKE_CMD_HELP_HPP

#include "io/memory.hpp"
#include <vector>
#include <string>

namespace voke::cmd::help {
  const static std::vector<std::string> alias {
    "--help", "-h"
  }; 

  voke::flags_t assert();
  voke::flags_t run();
}

#endif
