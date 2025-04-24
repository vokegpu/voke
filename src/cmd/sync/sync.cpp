#include "sync.hpp"
#include "voke.hpp"
#include "io/log.hpp"
#include "platform/git.hpp"
#include "platform/os.hpp"
#include "io/resource.hpp"
#include <iostream>

voke::flags_t voke::cmd::sync::assert() {
  if (voke::app.args.empty()) {
    return voke::result::PASS;
  }

  std::vector<voke::io::argument_t> args {
    voke::io::args_find_all(
      voke::cmd::sync::alias
    )
  };

  if (
    args.empty()
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
    voke::io::args_find_all({"-b", "--binary"}).size() == 0
    ||
    (
      voke::io::args_find_all({"-b", "--binary"}).size() == 1
      &&
      voke::io::args_find_all({"-b", "--binary"}).at(0).values.empty()
    )
  };

  bool is_version {
    voke::io::args_find_all({"-v", "--version"}).size() == 0
    ||
    (
      voke::io::args_find_all({"-v", "--version"}).size() == 1
      &&
      !voke::io::args_find_all({"-v", "--version"}).at(0).values.empty()
    )
  };

  if (
    voke::io::args_find_all({"-sc", "--sync-compiler"}).size() == 1
    &&
    !voke::io::args_find_all({"-sc", "--sync-compiler"}).at(0).values.empty()
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
    voke::io::args_find_all({"-s", "--sync"}).size() == 1
    &&
    voke::io::args_find_all({"-s", "--sync"}).at(0).values.size() == 1
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
  if (std::system("git --version") != 0) {
    return voke::result::SUCCESS;
  }

  voke::platform::voke_system_git_sync();
  std::string arg_builder {};

  std::ifstream installed_compilers(voke::platform::voke_system_installed_compilers_path);
  if (installed_compilers.is_open()) {
    std::string line {};
    while (getline(installed_compilers, line)) {
      if (line.empty()) {
        continue;
      }

      voke::io::extract_compiler_info_from_line(line, voke::app.compilers.emplace_back());
    }
  }

  std::vector<voke::io::argument_t> args {};
  if ((args = voke::io::args_find_all({"-s", "--sync"})).size() == 1) {
    voke::io::library_t library {
      .voke_tag = args.at(0).values.at(0)
    };

    library.voke_path += voke::platform::voke_system_path;
    library.voke_path += "/";
    library.voke_path += library.voke_tag;

    arg_builder = {};
    arg_builder += "cd ";
    arg_builder += library.voke_path;

    voke::log::status = std::system(arg_builder.c_str());

    if (voke::log::status != 0) {
      voke::log() << "\n---\n\nerror: no C/C++ library found named '" << library.voke_tag << "'";
      return voke::result::SUCCESS;
    }

    voke::log() << "\n---\n\ndetail: C/C++ library named '" << library.voke_tag << "' found";
    voke::log() << "detail: syncing library '" << library.voke_path << "'";

    
  }

  if (voke::io::args_find_all({"-sc", "--sync-compiler"}).size() == 1) {
    
  }

  return voke::result::SUCCESS;
}
