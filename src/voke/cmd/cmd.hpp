#ifndef VOKE_CMD_HPP
#define VOKE_CMD_HPP

#include "voke/cmd/help/help.hpp"
#include "voke/cmd/sync/sync.hpp"
#include "voke/cmd/version/version.hpp"
#include "voke/cmd/test/test.hpp"

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
