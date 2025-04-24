#include "sync.hpp"

#include "voke.hpp"
#include "io/log.hpp"

voke::flags_t voke::cmd::sync::assert() {
  if (voke::app.args.empty()) {
    return voke::result::PASS;
  }

  std::vector<voke::io::argument_t> args {};

  if (
    voke::io::args_find_any(
      voke::cmd::sync::alias,
      args
    ) == voke::result::TIMEOUT
    ||
    voke::io::args_contains_any_non(
      voke::cmd::sync::alias
    ) == voke::result::SUCCESS
  ) {
    return voke::result::FAILED;
  }

  if (
    args.at(0).prefix != "-s"
    &&
    args.at(0).prefix != "--sync"
    &&
    args.at(0).prefix != "-sc"
    &&
    args.at(0).prefix != "--sync-compiler"
  ) {
    return voke::result::FAILED;
  }

  bool is_binary {
    voke::io::args_find_all({"-b", "--binary"}, args).size() == 0
    ||
    (
      voke::io::args_find_all({"-b", "--binary"}, args).size() == 1
      &&
      voke::io::args_find_all({"-b", "--binary"}, args).at(0).values.empty()
    )
  };

  bool is_version {
    voke::io::args_find_all({"-v", "--version"}, args).size() == 0
    ||
    (
      voke::io::args_find_all({"-v", "--version"}, args).size() == 1
      &&
      !voke::io::args_find_all({"-v", "--version"}, args).at(0).values.empty()
    )
  };

  if (
    voke::io::args_find_all({"-sc", "--sync-compiler"}, args).size() == 1
    &&
    !voke::io::args_find_all({"-sc", "--sync-compiler"}, args).at(0).values.empty()
    &&
    (
      voke::app.args.size() == 1
      ||
      (is_binary && is_version)
      ||
      (is_version && is_binary)
    )
  ) {
    return voke::result::SUCCESS;
  }

  if (
    voke::io::args_find_all({"-s", "--sync"}, args).size() == 1
    &&
    !voke::io::args_find_all({"-s", "--sync"}, args).at(0).values.empty()
    &&
    (
      voke::app.args.size() == 1
      ||
      (is_binary && is_version)
      ||
      (is_version && is_binary)
    )
  ) {
    return voke::result::SUCCESS;
  }

  return voke::result::FAILED;
}

voke::flags_t voke::cmd::sync::run() {
  return voke::result::SUCCESS;
}
