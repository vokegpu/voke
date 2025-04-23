#include "help.hpp"
#include "voke.hpp"
#include "io/log.hpp"

voke::flags_t voke::cmd::help::assert() {
  if (voke::app.args.empty()) {
    return voke::result::PASS;
  }

  voke::io::argument_t argument {};

  if (
    voke::io::find_argument_by_prefix(
      voke::cmd::help::alias,
      argument
    ) == voke::result::TIMEOUT
  ) {
    return voke::result::FAILED;
  }

  if (!argument.values.empty()) {
    return voke::result::FAILED;
  }

  std::string help {
R"(><

If you could not understand how to use, visit:
https://github.com/vokegpu/voke-docs

Use:
  voke -v                 Output software version.
  voke --version

  voke -sc *              Sync the system with a specific compiler: clang, mingw, gnu.
  voke --sync-compiler *

  voke -s *               Sync the system with a C/C++ library.
  voke --sync *

  voke                    Upgrade all unsynced C/C++ libraries and compilers.
  voke -su
  voke --sync-upgrade 

  voke -r *               Remove a C/C++ library or a compiler.
  voke --remove *

Version:)"
  };

  voke::log() << help << "\n  " << voke::app.version;

  return voke::result::SUCCESS;
}