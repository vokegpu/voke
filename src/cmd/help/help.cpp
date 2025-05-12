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
      {{"-h", "--help"}, voke::empty, voke::behavior::MANDATORY}
    }
  };

  return voke::argument::compile(
    compiler,
    voke::app.args
  );
}

voke::flags_t voke::cmd::help::run() {
  std::string help {
R"(🐈‍⬛
sync a C/C++ library or compiler:
  voke |-s *
       |--sync *
       optional --- version to look-up:
         | -v *         
         | --version *
       optional --- pre-compiled binary:
         | -b
         | --binary
       optional --- specify compilers to sync the library:
         | -t
         | --targets
       optional --- force sync (re-installing):
         | -f
         | --force

remove a C/C++ library or compiler from the system:
  voke | -r *
       | --remove *

sync all C/C++ compilers/libraries installed under system:
  voke | -sac                  
       | --sync-all-compilers
       | -sal                  
       | --sync-all-libraries

output software version:
  voke | -v                    
       | --version

optional --- expose shell commands outputs:
 voke | -el
      | --extra-logs

for more development info visit:
| - | https://github.com/vokegpu/voke
| - | https://github.com/vokegpu/voke-docs)"
  };

  voke::log() << help;
  return voke::result::SUCCESS;
}

