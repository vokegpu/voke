#include "sync.hpp"
#include "voke.hpp"
#include "io/log.hpp"
#include "io/shell.hpp"
#include "platform/git.hpp"
#include "platform/os.hpp"
#include "resource/resource.hpp"
#include "io/vokefile.hpp"
#include <iostream>

voke::cmd_sync_mode voke::cmd::sync::mode {};

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
    voke::cmd::sync::mode = (
      voke::argument::find({"-s", "--sync"}).values.at(0).find("lib") != std::string::npos
      ?
      voke::cmd_sync_mode::SYNC_LIBRARY : voke::cmd_sync_mode::SYNC_TARGET
    );

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
    voke::cmd::sync::mode = voke::cmd_sync_mode::SYNC_ALL_TARGETS;
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
    voke::cmd::sync::mode = voke::cmd_sync_mode::SYNC_ALL_LIBRARIES;
    return voke::result::SUCCESS;
  }

  return voke::result::ERROR_UNKNOWN;
}

voke::flags_t voke::cmd::sync::run() {
  voke::log() << "detail: syncing voke-system repositories...";

  if (
    voke::platform::sync_git_repository(
      voke::platform::vokegpu_voke_libraries_repository_url,
      voke::system_dir_path
    ) != voke::result::SUCCESS;
  ) {
    voke::log() << "error: could not sync voke-system repositories, use -el or --extra-logs";
    return voke::result::ERROR_FAILED;
  }

  // @TODO: add logs for installed resources
  voke::platform::voke_system_fetch_installed_compilers();
  voke::platform::voke_system_fetch_installed_libraries();

  switch (voke::cmd::sync::mode) {
    case voke::cmd_sync_mode::SYNC_LIBRARY: {
      voke::argument_t &sync_argument {
        voke::argument::find({"-s", "--sync"})
      };

      if (sync_argument == voke::argument::not_found) {
        voke::log() << "fatal: could not find sync argument after assertation (?)";
        return voke::result::ERROR_FAILED;
      }

      std::string &sync_tag {sync_argument.values.at(0)};
      voke::library_t host_library {
        {"sync-tag": sync_tag},
        {"sync-dir": voke::system_dir_path + sync_tag},
      };

      voke::shell() << "cd " << host_library["sync-dir"];
      if (voke::shelll::result != 0) {
        voke::log() << "error: could not found any library named '" << sync_tag << "' under Vokegpu repositories";
        return voke::result::ERROR_FAILED;
      }

      std::vector<std::string> compiler_targets_vokefile_path {};
      VOKE_ASSERT(
        voke::io::vokefile_query_files_from_dir(
            host_library["sync-dir"],
            compiler_targets_vokefile_path
        ),
        voke::log() << "error: host library '" << sync_tag << "' has no C/C++ targets",
        voke::result::ERROR_FAILED
      );

      std::vector<voke::target_t> &compiler_targets {
        voke::app.targets[sync_tag]
      };

      for (std::string &vokefiles_path : compiler_targets) {
        voke::target_t &target {
          compiler_targets.emplace_back()
        };


      }

      VOKE_ASSERT(
        voke::platform::sync_git_repository(
          host_library["voke-respositories-url"],
          host_library["cached-dir"]
        ),
        voke::log() << "error: could not clone host-library '" << sync_tag << "' url '" >> host_library[""],
        voke::result::ERROR_FAILED
      );

      voke::library_t local_library {};


      voke::argument_t &version_argument {
        voke::argument::find({"-v", "--version"})
      };

      if (version_argument == voke::argument::not_found) {
        library[""]
      }

      voke::argument_t &binary_argument {
        voke::argument::find({"-b", "--binary"})
      };

      if (binary_argument ==  voke::argument::not_found) {

      } else {

      }

      break;
    }

    case voke::cmd_sync_mode::SYNC_TARGET: {
      break;
    }

    case voke::cmd_sync_mode::SYNC_ALL_LIBRARIES: {
      break;
    }

    case voke::cmd_sync_mode::SYNC_ALL_TARGETS: {
      break;
    }
  }

  return voke::result::SUCCESS;
}
