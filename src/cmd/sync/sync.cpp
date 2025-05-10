#include "sync.hpp"
#include "voke.hpp"
#include "io/log.hpp"
#include "io/shell.hpp"
#include "platform/git.hpp"
#include "platform/os.hpp"
#include "resource/resource.hpp"
#include <iostream>

voke::flags_t voke::cmd::sync::assert() {
  if (voke::app.raw_args.empty()) {
    return voke::result::SUCCESS_PASS;
  }

  voke::argument_compiler_info_t compiler_info {
    .tag = "sync",
    .lines = voke::app.raw_args,
    .match_first = true,
    .expect = {
      {{"-s", "--sync"}, 1, voke::behavior::MANDATORY},
      {{"-v", "--version"}, 1, voke::behavior::OPTIONAL},
      {{"-b", "--binary"}, voke::empty, voke::behavior::OPTIONAL},
      {{"-t", "--targets"}, voke::empty, voke::behavior::OPTIONAL},
      {{"-el", "--extra-logs"}, voke::empty, voke::behavior::OPTIONAL}
    }
  };

  if (
    voke::argument::compile(
      compiler_info,
      voke::app.args
    ) == voke::result::SUCCESS
  ) {
    return voke::result::SUCCESS;
  }

  compiler_info = {
    .tag = "sync-all-targets",
    .lines = voke::app.raw_args,
    .match_first = true,
    .expect = {
      {{"-sat", "--sync-all-targets"}, voke::empty, voke::behavior::MANDATORY},
      {{"-el", "--extra-logs"}, voke::empty, voke::behavior::OPTIONAL}
    }
  };

  if (
    voke::argument::compile(
      compiler_info,
      voke::app.args
    ) == voke::result::SUCCESS
  ) {
    return voke::result::SUCCESS;
  }

  compiler_info = {
    .tag = "sync-all-libraries",
    .lines = voke::app.raw_args,
    .match_first = true,
    .expect = {
      {{"-sal", "--sync-all-libraries"}, voke::empty, voke::behavior::MANDATORY},
      {{"-el", "--extra-logs"}, voke::empty, voke::behavior::OPTIONAL}
    }
  };

  if (
    voke::argument::compile(
      compiler_info,
      voke::app.args
    ) == voke::result::SUCCESS
  ) {
    return voke::result::SUCCESS;
  }

  return voke::result::SUCCESS_PASS;
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
      voke::system_dir_path
    ) == voke::result::ERROR_FAILED
  ) {
    voke::log() << "error: could not sync voke-system libraries repository use -el or --extra-logs";
    return voke::result::SUCCESS;
  }

  voke::platform::voke_system_fetch_installed_compilers();
  voke::platform::voke_system_fetch_installed_libraries();

  std::string arg_builder {};
  std::vector<voke::argument_t> args {voke::argument::find({"-s", "--sync"})};
  if (args.size() == 1 && args.at(0).values.size() == 1 && args.at(0).values.at(0).size() > 3) {
    voke::argument_t arg {
      args.at(0)
    };

    bool is_library_type {
      arg.values.at(0).at(0) == 'l'
      &&
      arg.values.at(0).at(1) == 'i'
      &&
      arg.values.at(0).at(2) == 'b'
    };

    if (
      is_library_type
    ) {
      voke::library_t library {
        .voke_tag = args.at(0).values.at(0)
      };

      voke::log() << "detail: searching for library named '" << static_cast<std::string>(library["tag"]) << "'...";

      library["path"] += voke::system_dir_path;
      library["path"] += "/";
      library["path"] += library["tag"];

      voke::shell() << "cd " << static_cast<std::string>(library["path"]);
      if (voke::shell::result != 0) {
        voke::log() << "error: no library found named '" << static_cast<std::string>(library["tag"]) << "'";
        return voke::result::SUCCESS;
      }

      voke::log() << "detail: found, synching at '" << static_cast<std::string>(library["path"]) << '\''; 

      library.repository_cache_path = (
        voke::system_cached_repositories_dir_path
        +
        library["tag"]
      );

      args = voke::argument::find({"-v", "--version"});
      if (args.size() == 1) {

      }

      args = voke::argument::find({"-b", "--binary"});
      if (args.size() == 1) {
        
      }

      if (args.empty()) {
        voke::log() << "detail: looking for voke-files...";

        std::vector<std::string> lookup_compilers_voke_file {};
        voke::platform::voke_system_lookup_targets_from_library(
          library,
          lookup_compilers_voke_file
        );

        voke::platform::voke_system_fetch_compilers_info_from_host_library(
          library,
          lookup_compilers_voke_file
        );

        voke::log() << "detail: synching at '" << library.repository_cache_path << '\'';

        if (
          voke::platform::sync_git_repository(
            library.url,
            library.repository_cache_path,
            library.git_clone_args
          ) == voke::result::ERROR_FAILED
        ) {
          voke::log() << "error: could not sync repository '" << library.url << "' use -el or --extra-logs";
          return voke::result::SUCCESS;
        }

        voke::platform::compile_libraries();
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
