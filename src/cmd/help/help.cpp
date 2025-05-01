#include "help.hpp"
#include "voke.hpp"
#include "io/log.hpp"

voke::flags_t voke::cmd::help::assert() {
  if (voke::app.args.empty()) {
    return voke::result::SUCCESS_PASS;
  }

  std::vector<voke::argument_t> args {
    voke::io::args_find_all(
      voke::cmd::help::alias
    )
  };

  if (args.empty()) {
    return voke::result::ERROR_FAILED;
  }

  if (
    args.empty()
    ||
    voke::io::args_contains_any_non(
      voke::cmd::help::alias
    ) == voke::result::SUCCESS
  ) {
    return voke::result::ERROR_FAILED;
  }

  if (
      !args.at(0).values.empty()
      ||
      (
        voke::app.args.at(0).prefix != "-h"
        &&
        voke::app.args.at(0).prefix != "--help"
      )
    ) {
    return voke::result::ERROR_FAILED;
  }

  return voke::result::SUCCESS;
}

voke::flags_t voke::cmd::help::run() {
  std::string help {
R"(detail:

><

for more development info visit:
| - | https://github.com/vokegpu/voke
| - | https://github.com/vokegpu/voke-docs

use:
  voke | -s *                  sync a C/C++ library or a compiler
       | --sync *
        | -v *                 optional: version to look-up
        | --version *
        | -b                   optional: pre-compiled binary
        | --binary
        | -c                   optional: specify compilers to sync the library, e.g: clang-msvc64 clang-mingw64 etc 
        | --compilers

  voke | -r *                  remove a C/C++ library or a compiler from the system
       | --remove *

  voke | -sac                  sync all C/C++ compilers-installed in voke-system 
       | --sync-all-compilers
       | -sal                  sync all C/C++ libraries-installed in voke-system 
       | --sync-all-libraries

  voke | -v                    output software version
       | --version

version:)"
  };

  voke::log() << help << "\n  " << voke::app.version;
  return voke::result::SUCCESS;
}

