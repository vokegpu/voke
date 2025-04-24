#include "version.hpp"
#include "voke.hpp"
#include "io/log.hpp"

voke::flags_t voke::cmd::version::assert() {
  if (voke::app.args.empty()) {
    return voke::result::PASS;
  }

  std::vector<voke::io::argument_t> args {
    voke::io::args_find_all(
      voke::cmd::version::alias
    )
  };

  if (args.empty()) {
    return voke::result::FAILED;
  }

  if (
      !args.at(0).values.empty()
      ||
      voke::app.args.size() != 1
    ) {
    return voke::result::FAILED;
  }

  return voke::result::SUCCESS;
}

voke::flags_t voke::cmd::version::run() {
  voke::log() << ">< meow moo\nVersion " << voke::app.version << "\n@VokeGpu";
  return voke::result::SUCCESS;
}
