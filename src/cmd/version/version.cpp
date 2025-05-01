#include "version.hpp"
#include "voke.hpp"
#include "io/log.hpp"

voke::flags_t voke::cmd::version::assert() {
  if (voke::app.args.empty()) {
    return voke::result::SUCCESS_PASS;
  }

  std::vector<voke::argument_t> args {
    voke::io::args_find_all(
      voke::cmd::version::alias
    )
  };

  if (args.empty()) {
    return voke::result::ERROR_FAILED;
  }

  if (
      !args.at(0).values.empty()
      ||
      voke::app.args.size() != 1
    ) {
    return voke::result::ERROR_FAILED;
  }

  return voke::result::SUCCESS;
}

voke::flags_t voke::cmd::version::run() {
  voke::log() << "detail: voke (Voke) version " << voke::app.version;
  voke::log() << "detail: https://github.com/vokegpu/voke";
  voke::log() << "detail: @vokegpu";
  return voke::result::SUCCESS;
}
