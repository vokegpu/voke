#ifndef VOKE_CMD_SYNC_HPP
#define VOKE_CMD_SYNC_HPP

#include "io/memory.hpp"

#include <vector>
#include <string>

namespace voke::cmd::sync {
  const static std::vector<std::string> alias {
    "-s", "--sync",
    "-b", "--binary",
    "-v", "--version",
    "-c", "--compilers",
    "-sal", "--sync-all-libraries",
    "-sac", "--sync-all-compilers"
  };

  voke::flags_t assert();
  voke::flags_t run();
}

#endif
