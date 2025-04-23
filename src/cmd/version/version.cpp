#include "version.hpp"
#include "voke.hpp"
#include "io/log.hpp"

voke::flags_t voke::cmd::version::assert() {
  if (voke::app.args.empty()) {
    return voke::result::PASS;
  }

  voke::io::argument_t argument {};

  if (
    voke::io::find_argument_by_prefix(
      voke::cmd::version::alias,
      argument
    ) == voke::result::TIMEOUT
  ) {
    return voke::result::FAILED;
  }

  if (!argument.values.empty()) {
    return voke::result::FAILED;
  }

  voke::log() << ">< meow moo\nVersion " << voke::app.version << "\n@VokeGpu";

  return voke::result::SUCCESS;
}