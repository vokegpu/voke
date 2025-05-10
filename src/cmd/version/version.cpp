#include "version.hpp"
#include "voke.hpp"
#include "io/log.hpp"

voke::flags_t voke::cmd::version::assert() {
  if (voke::app.raw_args.empty()) {
    return voke::result::SUCCESS;
  }

  voke::argument_compiler_info_t compiler_info {
    .tag = "version",
    .lines = voke::app.raw_args,
    .match_first = true,
    .expect = {
      {{"-v", "--version"}, voke::empty, voke::behavior::MANDATORY}
    }
  };

  return voke::argument::compile(
    compiler_info,
    voke::app.args
  );
}

voke::flags_t voke::cmd::version::run() {
  voke::log() << "detail: voke (Voke) version " << voke::app.version;
  voke::log() << "detail: https://github.com/vokegpu/voke";
  voke::log() << "detail: @vokegpu";
  return voke::result::SUCCESS;
}
