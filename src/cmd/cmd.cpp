#include "cmd.hpp"
#include "voke.hpp"
#include "io/shell.hpp"
#include "io/log.hpp"

bool voke::cmd::tweaks() {
  voke::argument_t &extra_logs_argument {voke::argument::find({"-el", "--extra-logs"})};
  if (extra_logs_argument != voke::argument::not_found) {
    uint8_t layer {static_cast<uint8_t>(std::stoi(static_cast<std::string&>(extra_logs_argument)))};
    if (layer < 2 || layer > 4) {
      voke::log() << "error: invalid extra logs level, please try a number between 2 - 4";
      return false;
    }

    voke::io_verbose_level = --layer;
  }

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
