#include "io/log.hpp"
#include "argument/argument.hpp"
#include "io/memory.hpp"
#include "voke.hpp"

#include "cmd/help/help.hpp"
#include "cmd/version/version.hpp"
#include "cmd/sync/sync.hpp"
#include "platform/git.hpp"

#include <iostream>

voke::app_t voke::app {};

int32_t main(int32_t args_size, char **pp_args) {
  voke::app.raw_args(pp_args, pp_args + args_size);

  voke::cmd::add(
    voke::cmd::version::assert,
    voke::cmd::version::run
  );

  voke::cmd::add(
    voke::cmd::sync::assert,
    voke::cmd::sync::run
  );

  voke::cmd::add(
    voke::cmd::help::assert,
    voke::cmd::help::run
  );

  if (voke::app.commands_state == false) {
    voke::argument::throw_unknown_command_or_arguments();
  }

  return voke::log::flush();
}
