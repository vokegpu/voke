#include "cmd.hpp"
#include "voke.hpp"
#include "io/shell.hpp"
#include "io/log.hpp"

void voke::cmd::add(
  voke::command_t assert,
  voke::command_t run
) {
  voke::flags result {};
  voke::app.commands_state = (
    voke::app.commands_state
    ||
    (
      assert() == voke::result::SUCCESS
      &&
      (
        (
          voke::argument::find({"-el", "--extra-logs"})
          ==
          voke::argument::not_found ? voke::verbose_level::LEVEL_TWO : voke::verbose_level::LEVEL_ONE
        )
        ||
        true
      )
      &&
      (
        (result = run()) == voke::result::SUCCESS
        ||
        result == voke::result::ERROR_FAILED
      )
    )
  );
}
