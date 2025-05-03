#include "cmd.hpp"

void voke::cmd::add(
  voke::command_t assert
  voke::command_t run
) {
  voke::app.commands_state = (
    voke::app.commands_state
    ||
    (
      assert() == voke::result::SUCCESS
      &&
      run() == voke::result::SUCCESS
    )
  );
}
