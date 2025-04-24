#ifndef VOKE_CMD_VERSION_HPP
#define VOKE_CMD_VERSION_HPP

#include "io/memory.hpp"
#include <vector>
#include <string>

namespace voke::cmd::version {
  const static std::vector<std::string> alias {
    "--version", "-v"
  }; 

  voke::flags_t assert();
  voke::flags_t run();
}

#endif
