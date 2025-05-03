#include "io/log.hpp"
#include "cmd/cmd.hpp"
#include "voke.hpp"
#include "platform/git.hpp"

#include <iostream>

voke::app_t voke::app {};

int32_t main(int32_t args_size, char **pp_args) {
  voke::app.raw_args = {pp_args, pp_args + args_size};
  voke::app.raw_args.erase(voke::app.raw_args.begin());

  std::string raw {};
  for (std::string &raw_arg : voke::app.raw_args) {
    raw += raw_arg;
    raw += ' ';
  }

  voke::app.raw_args.clear();
  voke::app.raw_args.emplace_back() = raw;

  //voke::cmd::add(
  //  voke::cmd::version::assert,
  //  voke::cmd::version::run
  //);

  voke::cmd::add(
    voke::cmd::sync::assert,
    voke::cmd::sync::run
  );

  //voke::cmd::add(
  //  voke::cmd::help::assert,
  //  voke::cmd::help::run
  //);

  if (voke::app.commands_state == false) {
    voke::argument::throw_unknown_command_or_arguments();
  }

  return voke::log::flush();
}
