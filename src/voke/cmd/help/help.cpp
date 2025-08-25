#include "help.hpp"
#include "voke/voke.hpp"
#include "voke/io/log.hpp"

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
R"(template:
  voke [command] <values> [options]
   | [option]
   | [option] <values>
commands:
  voke [-s --sync] <library/compiler>
   | [-v --version] <version> 
   | [-b --binary]
   | [-t --targets] <compilers> 
   | [-f --force] 
   | [-el --extra-logs]
   | [-d --debug]
  voke [-r --remove] <library/compiler>
   | [-t --targets] <compilers>
   | [-el --extra-logs]
  voke [-sat --sync-all-targets]
   | [-el --extra-logs]
  voke [-sal --sync-all-libraries]
   | [-el --extra-logs]
  voke [-v --version])"};

  voke::log() << help;
  return voke::result::SUCCESS;
}

