#include "test.hpp"
#include "voke/voke.hpp"
#include "voke/io/log.hpp"

voke::flags_t voke::cmd::test::assert() {
  if (voke::app.raw_args.empty()) {
    return voke::result::SUCCESS;
  }

  voke::argument_compiler_info_t compiler_info {
    .tag = "test",
    .lines = voke::app.raw_args,
    .match_first = true,
    .expect = {
      {{"--test"}, voke::empty, voke::behavior::MANDATORY}
    }
  };

  return voke::argument::compile(
    compiler_info,
    voke::app.args
  );
}

voke::flags_t voke::cmd::test::run() {
  voke::log() << "testig stuff...";

  return voke::result::SUCCESS;
}
