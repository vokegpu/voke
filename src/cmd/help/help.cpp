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

For more development info visit:
| - | https://github.com/vokegpu/voke
| - | https://github.com/vokegpu/voke-docs

Use:
  voke -v                 Output software version.
  voke --version

  voke -sc *              Sync the system with a compiler: clang, mingw, clang-mingw, gnu.
  voke --sync-compiler *
  voke ... -v *           Sync the system with a specific version of a compiler.
  voke ... --version *
  voke ... ... -b         Sync the system with a compiler pre-compiled binary, you can combine with version.
  voke ... ... --binary

  voke -s *               Sync the system with a C/C++ library.
  voke --sync *
  voke ... -v             Sync the system with a specific version of a C/C++ library.
  voke ... --version *
  voke ... ... -b         Sync the system with a C/C++ library pre-compiled binary, you can combine with version.
  voke ... ... --binary

  voke                    Upgrade all unsynced C/C++ libraries and compilers in the system.
  voke -su
  voke --sync-upgrade 

  voke -r *               Remove a C/C++ library or a compiler from the system.
  voke --remove *

Version:)"
  };

  voke::log() << help << "\n  " << voke::app.version;

  return voke::result::SUCCESS;
}