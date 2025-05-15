#include "io/log.hpp"
#include "cmd/cmd.hpp"
#include "voke.hpp"
#include "platform/git.hpp"
#include "io/shell.hpp"
#include "platform/os.hpp"

#include <iostream>

voke::app_t voke::app {};

int32_t main(int32_t args_size, char **pp_args) {
  voke::app.raw_args = {pp_args, pp_args + args_size};
  voke::app.raw_args.erase(voke::app.raw_args.begin());

  std::string raw {};
  for (std::string &raw_arg : voke::app.raw_args) {
    if (raw_arg.find(" ") != std::string::npos) {
      if (raw_arg.find("\"") != std::string::npos) {
        raw += '\'';
        raw += raw_arg;
        raw += "' ";
      } else {
        raw += '"';
        raw += raw_arg;
        raw += "\" ";
      }
    } else {
      raw += raw_arg;
      raw += ' ';
    }
  }

  voke::app.raw_args.clear();
  voke::app.raw_args.emplace_back() = raw;

  voke::shell() << "git --version";
  if (voke::shell::result != 0) {
    voke::log() << "fatal: command 'git' not found";
    return voke::log::flush();
  }

  if (voke::platform::voke_system_init() != voke::result::SUCCESS) {
    voke::log() << "fatal: could not initialize voke-system";
    return voke::log::flush();
  }

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
