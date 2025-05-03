#include "help.hpp"
#include "voke.hpp"
#include "io/log.hpp"

voke::flags_t voke::cmd::help::assert() {
  if (voke::app.raw_args.empty()) {
    return voke::result::SUCCESS;
  }

  voke::argument_compiler_info_t compiler {
    .tag = "help",
    .lines = voke::app.raw_args,
    .match_first = true,
    .expect = {
      {{"-h", "--help"}, 1, voke::mandatory}
    }
  };

  return voke::argument::compile(
    compiler,
    voke::app.args
  );
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

