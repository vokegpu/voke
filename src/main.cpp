#include "io/log.hpp"
#include "io/git.hpp"
#include "io/args.hpp"
#include "voke.hpp"
#include "cmd/version/version.hpp"

voke::app_t voke::app {};

#include <iostream>

int32_t main(int32_t args_size, char **pp_args) {
  voke::io::fill(args_size, pp_args);

  for (voke::io::argument_t &arg : voke::app.args) {
    if (arg.raw.empty()) {
      continue;
    }
  }

  if (voke::cmd::version::assert()) {
    
  }

  return voke::log::flush();
}