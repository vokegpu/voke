#ifndef VOKE_CMD_SYNC_HPP
#define VOKE_CMD_SYNC_HPP

#include "io/memory.hpp"

#include <vector>
#include <string>

namespace voke::cmd::sync {
  const static std::vector<std::string> alias {
    "-s",  "--sync",
    "-sc", "--sync-compiler",
    "-b",  "--binary",
    "-v",  "--version"
  };

  voke::flags_t assert();
  voke::flags_t run();
}

#endif
