#ifndef VOKE_CMD_HPP
#define VOKE_CMD_HPP

#include "cmd/help/help.hpp"
#include "cmd/sync/sync.hpp"
#include "cmd/version/version.hpp"

namespace voke {
  typedef voke::flags_t(*command_t)();
}

namespace voke::cmd {
  bool tweaks();

  void add(
    voke::command_t command,
    voke::command_t run
  );
}

#endif
