#include "io/log.hpp"
#include "io/git.hpp"
#include "io/args.hpp"
#include "io/memory.hpp"
#include "voke.hpp"

#include "cmd/help/help.hpp"
#include "cmd/version/version.hpp"

#include <iostream>

voke::app_t voke::app {};

int32_t main(int32_t args_size, char **pp_args) {
  voke::io::fill(args_size, pp_args);

  bool status_ok {};

  status_ok = (
    voke::cmd::version::assert() == voke::result::SUCCESS
    &&
    voke::cmd::version::run() == voke::result::SUCCESS
  );

  status_ok = (
    voke::cmd::help::assert() == voke::result::SUCCESS
    &&
    voke::cmd::help::run() == voke::result::SUCCESS
  );

  if (status_ok == false) {
    voke::log() << "Unknown argument use --help or -h";
  }

  return voke::log::flush();
}
