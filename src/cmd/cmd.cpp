#include "cmd.hpp"
#include "voke.hpp"
#include "io/shell.hpp"
#include "io/log.hpp"

void voke::cmd::add(
  voke::command_t assert,
  voke::command_t run
) {
  voke::app.commands_state = (
    voke::app.commands_state
    ||
    (
      assert() == voke::result::SUCCESS
      &&
      (
        (
          voke::shell::verbose_level = voke::argument::find({"-el", "--extra-logs"}).size()
          ==
          1 ? voke::verbose_level::LEVEL_TWO : voke::verbose_level::LEVEL_ONE
        )
        ||
        true
      )
      &&
      run() == voke::result::SUCCESS
    )
  );
}
