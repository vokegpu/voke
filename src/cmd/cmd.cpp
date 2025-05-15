#include "cmd.hpp"
#include "voke.hpp"
#include "io/shell.hpp"
#include "io/log.hpp"

bool voke::cmd::tweaks() {
  voke::shell::verbose_level = (
    voke::argument::find({"-el", "--extra-logs"})
    ==
    voke::argument::not_found ?
    voke::verbose_level::LEVEL_ONE : voke::verbose_level::LEVEL_TWO
  );

  voke::log::debug = (
    voke::argument::find({"-d", "--debug"})
    !=
    voke::argument::not_found
  );

  return true;
}

void voke::cmd::add(
  voke::command_t assert,
  voke::command_t run
) {
  voke::flags_t result {};
  voke::app.commands_state = (
    voke::app.commands_state
    ||
    (
      assert() == voke::result::SUCCESS
      &&
      voke::cmd::tweaks()
      &&
      (
        (result = run()) == voke::result::SUCCESS
        ||
        result == voke::result::ERROR_FAILED
      )
    )
  );
}
