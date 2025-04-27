#include "sync.hpp"
#include "voke.hpp"
#include "io/log.hpp"
#include "platform/git.hpp"
#include "platform/os.hpp"
#include "io/resource.hpp"
#include <iostream>

voke::flags_t voke::cmd::sync::assert() {
  if (voke::app.args.empty()) {
    return voke::result::SUCCESS_PASS;
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
    return voke::result::ERROR_FAILED;
  }

  if (
    args.at(0).prefix != "-s"
    &&
    args.at(0).prefix != "--sync"
    &&
    args.at(0).prefix != "--sac"
    &&
    args.at(0).prefix != "--sync-all-compilers"
    &&
    args.at(0).prefix != "-sal"
    &&
    args.at(0).prefix != "--sync-all-libraries"
  ) {
    return voke::result::ERROR_FAILED;
  }

  args = voke::io::args_find_all({"-s", "--sync"});
  if (args.size() == 1) {
    if (args.at(0).values.size() != 1) {
      return voke::result::ERROR_FAILED;
    }

    args = voke::io::args_find_all({"-v", "--version"});
    if (!args.empty() && (args.size() != 1 || args.at(0).values.empty())) {
      return voke::result::ERROR_FAILED;
    }

    args = voke::io::args_find_all({"-b", "--binary"});
    if (!args.empty() && (args.size() != 1 || !args.at(0).values.empty())) {
      return voke::result::ERROR_FAILED;
    }

    args = voke::io::args_find_all({"-c", "--compilers"});
    if (!args.empty() && (args.size() != 1 || args.at(0).values.empty())) {
      return voke::result::ERROR_FAILED;
    }

    return voke::result::SUCCESS;
  }

  args = voke::io::args_find_all({"-sal", "--sync-all-libraries"});
  if (args.size() == 1) {
    if (!args.at(0).values.empty()) {
      return voke::result::ERROR_FAILED;
    }

    return voke::result::SUCCESS;
  }

  args = voke::io::args_find_all({"-sac", "--sync-all-compilers"});
  if (args.size() == 1) {
    if (!args.at(0).values.empty()) {
      return voke::result::ERROR_FAILED;
    }

    return voke::result::SUCCESS;
  }

  return voke::result::ERROR_FAILED;
}

voke::flags_t voke::cmd::sync::run() {
  voke::shell() << "git --version";
  if (voke::shell::result != 0) {
    voke::log() << "error: command 'git' not found, please install git";
    return voke::result::SUCCESS;
  }

  if (
    voke::platform::sync_git_repository(
      voke::platform::vokegpu_voke_libraries_repository_url,
      voke::platform::voke_system_path
    ) == voke::result::ERROR_FAILED
  ) {
    return voke::result::SUCCESS;
  }

  voke::platform::voke_system_fetch_installed_compilers();
  voke::platform::voke_system_fetch_installed_libraries();

  std::string arg_builder {};
  std::vector<voke::io::argument_t> args {voke::io::args_find_all({"-s", "--sync"})};
  if (args.size() == 1 && args.at(0).values.size() == 1 && args.at(0).values.at(0).size() > 3) {
    voke::io::argument_t arg {
      args.at(0)
    };

    bool is_library_type {
      arg.values.at(0).at(0) == 'l' // meow
      &&
      arg.values.at(0).at(1) == 'i' // moo
      &&
      arg.values.at(0).at(2) == 'b' // wolfowlf
    };

    if (
      is_library_type
    ) {
      voke::io::library_t library {
        .voke_tag = args.at(0).values.at(0)
      };

      library.voke_path += voke::platform::voke_system_path;
      library.voke_path += "/";
      library.voke_path += library.voke_tag;

      voke::shell() << "cd " << library.voke_path;
      if (voke::shell::result != 0) {
        voke::log() << "error: no C/C++ library found named '" << library.voke_tag << "'";
        return voke::result::SUCCESS;
      }

      voke::log() << "detail: C/C++ library named '" << library.voke_tag << "' found";
      voke::log() << "detail: syncing library '" << library.voke_path << "'";

      args = voke::io::args_find_all({"-v", "--version"});
      if (args.size() == 1) {

      }

      args = voke::io::args_find_all({"-b", "--binary"});
      if (args.size() == 1) {
        
      }

      if (args.size() == 0) {
        std::vector<std::string> lookup_compilers_voke_file {};
        voke::platform::voke_system_lookup_compilers_from_host_library(
          library,
          lookup_compilers_voke_file
        );

        voke::platform::voke_system_fetch_compilers_info_from_host_library(
          library,
          lookup_compilers_voke_file
        );
      }

      return voke::result::SUCCESS;
    }

    bool is_compiler_type {
      !is_library_type
    };

    if (is_compiler_type) {
        return voke::result::SUCCESS;
    }
  }

  return voke::result::SUCCESS;
}
