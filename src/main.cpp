#include "io/log.hpp"
#include "io/args.hpp"
#include "io/memory.hpp"
#include "voke.hpp"

#include "cmd/help/help.hpp"
#include "cmd/version/version.hpp"
#include "cmd/sync/sync.hpp"
#include "platform/git.hpp"

#include <iostream>

voke::app_t voke::app {};

int32_t main(int32_t args_size, char **pp_args) {
  std::vector<std::string> in_args {};
  voke::io::extract(args_size, pp_args, in_args);
  voke::io::fill(in_args, voke::app.args);

  std::vector<voke::argument_t> args {voke::io::args_find_all({"-el", "--extra-logs"})};
  if (args.size() == 1 && !args.at(0).values.empty()) {
    voke::io::throw_unknown_command_or_arguments();
    return voke::log::flush();
  }

  voke::app.verbose_level = (
    args.empty() ? voke::verbose_level::LEVEL_ONE : voke::verbose_level::LEVEL_TWO
  );

  bool status_ok {};

  status_ok = (
    voke::cmd::version::assert() == voke::result::SUCCESS
    &&
    voke::cmd::version::run() == voke::result::SUCCESS
  );

  status_ok = status_ok || (
    voke::cmd::help::assert() == voke::result::SUCCESS
    &&
    voke::cmd::help::run() == voke::result::SUCCESS
  );

  status_ok = status_ok || (
    voke::cmd::sync::assert() == voke::result::SUCCESS
    &&
    voke::cmd::sync::run() == voke::result::SUCCESS
  );

  if (status_ok == false) {
    voke::io::throw_unknown_command_or_arguments();
  }

  return voke::log::flush();
}
