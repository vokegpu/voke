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
  voke::argument_compiler_info_t compiler_info {
    .tag = "sync",
    .lines = voke::app.raw_args,
    .match_first = true,
    .expect = {
      {{"-s", "--sync"}, 1, voke::behavior::MANDATORY},
      {{"-v", "--version"}, 1, voke::behavior::OPTIONAL},
      {{"-b", "--binary"}, voke::empty, voke::behavior::OPTIONAL},
      {{"-t", "--targets"}, voke::empty, voke::behavior::OPTIONAL},
      {{"-f", "--force"}, voke::empty, voke::behavior::OPTIONAL},
      {{"-el", "--extra-logs"}, voke::empty, voke::behavior::OPTIONAL}
    }
  };

  VOKE_ASSERT(
    voke::argument::compile(
      compiler_info,
      voke::app.args
    ) == voke::result::SUCCESS,
    (
      voke::cmd::sync::mode = (
        voke::argument::find({"-s", "--sync"}).values.at(0).find("lib") != std::string::npos
        ?
        voke::cmd_sync_mode::SYNC_LIBRARY : voke::cmd_sync_mode::SYNC_TARGET
      )
    ),
    voke::result::SUCCESS
  );

  compiler_info = {
    .tag = "sync-all-targets",
    .lines = voke::app.raw_args,
    .match_first = true,
    .expect = {
      {{"-sat", "--sync-all-targets"}, voke::empty, voke::behavior::MANDATORY},
      {{"-el", "--extra-logs"}, voke::empty, voke::behavior::OPTIONAL},
      {{"-f", "--force"}, voke::empty, voke::behavior::OPTIONAL}
    }
  };

  VOKE_ASSERT(
    voke::argument::compile(
      compiler_info,
      voke::app.args
    ) == voke::result::SUCCESS,
    (voke::cmd::sync::mode = voke::cmd_sync_mode::SYNC_ALL_TARGETS),
    voke::result::SUCCESS
  );

  compiler_info = {
    .tag = "sync-all-libraries",
    .lines = voke::app.raw_args,
    .match_first = true,
    .expect = {
      {{"-sal", "--sync-all-libraries"}, voke::empty, voke::behavior::MANDATORY},
      {{"-el", "--extra-logs"}, voke::empty, voke::behavior::OPTIONAL},
      {{"-f", "--force"}, voke::empty, voke::behavior::OPTIONAL}
    }
  };

  VOKE_ASSERT(
    voke::argument::compile(
      compiler_info,
      voke::app.args
    ) == voke::result::SUCCESS,
    (voke::cmd::sync::mode = voke::cmd_sync_mode::SYNC_ALL_LIBRARIES),
    voke::result::SUCCESS
  );

  return voke::result::ERROR_UNKNOWN;
}

voke::flags_t voke::cmd::sync::run() {
  voke::log() << "detail: syncing voke-system repositories...";

  VOKE_ASSERT(
    voke::platform::sync_git_repository(
      voke::platform::vokegpu_voke_libraries_repository_url,
      voke::system_dir_path
    ),
    voke::log() << "error: could not sync voke-system repositories, use -el or --extra-logs",
    voke::result::ERROR_FAILED
  );

  voke::platform::voke_system_fetch_installed_compilers();
  voke::platform::voke_system_fetch_installed_libraries();

  switch (voke::cmd::sync::mode) {
    case voke::cmd_sync_mode::SYNC_LIBRARY: {
      voke::argument_t &sync_argument {
        voke::argument::find({"-s", "--sync"})
      };

      VOKE_ASSERT(
        sync_argument == voke::argument::not_found,
        voke::log() << "fatal: could not find sync argument after assertation (?)",
        voke::result::ERROR_FAILED
      );

      std::string &sync_tag {sync_argument};
      voke::library_t host_library {
        {"tag", sync_tag},
        {"sync-dir", voke::system_dir_path + sync_tag},
        {"version", "?"},
        {"cloned": "0"}
      };

      VOKE_ASSERT(
        voke::shell() << "cd " << host_library["sync-dir"],
        voke::log() << "error: could not find library named '" << sync_tag << '\'',
        voke::result::ERROR_FAILED
      );

      voke::argument_t &targets_argument {voke::io::find({"-t", "--targets"})};
      std::string not_found_target {};

      VOKE_ASSERT(
        (
          targets_argument != voke::argument::not_found
          &&
          std::find_if(
            targets_argument.values.begin(),
            targets_argument.values.end(),
            [](std::string &value) {
              voke::resource_query_info_t<voke::compiler_t> local_compiler_query_info {
                .predicate = [value](voke::compiler_t &compiler) {
                  return compiler["tag"] == value;
                },
                .resources = voke::app.installed_compilers
              };
            
              voke::compiler_t found_compiler {};
              return voke::resource::query<voke::compiler_t>(local_compiler_query_info, found_compiler);
            }
          ) == targets_argument.values.end()
        ),
        voke::log() << "error: could not find target named '" << not_found_target << "', please sync this target before use if not installed",
        voke::result::ERROR_FAILED
      );

      std::vector<std::string> compiler_targets_vokefile_path {};
      VOKE_ASSERT(
        voke::io::vokefile_query_files_from_dir(
            host_library["sync-dir"],
            compiler_targets_vokefile_path
        ),
        voke::log() << "error: host library '" << sync_tag << "' has no C/C++ targets",
        voke::result::ERROR_FAILED
      );

      std::vector<voke::target_t> &compiler_targets {voke::app.targets[sync_tag]};
      for (std::string &vokefiles_path : compiler_targets) {
        voke::target_t &target {
          compiler_targets.emplace_back()
        };

        VOKE_ASSERT(
          voke::platform::voke_system_fetch_library_target(
            host_library,
            target
          ),
          voke::log() << "error: could not fetch library target from '" << vokefiles_path << '\'',
          voke::result::ERROR_FAILED
        );
      }

      voke::log() << "detail: checking if library is already satisfied..."

      voke::library_t local_library {
        {"version", "?"}
      };

      voke::resource_query_info_t<voke::library_t> local_library_query_info {
        .predicate = [sync_tag](voke::library_t &library) {
          return sync_tag == library["tag"];
        },
        .resources = voke::app.installed_libraries
      };

      voke::resource::query<voke::library_t>(
        local_library_query_info,
        local_library
      );

      voke::argument_t &version_argument {voke::argument::find({"-v", "--version"})};
      if (version_argument == voke::argument::not_found) {
        // @TODO: add pick-version feature
        // @TODO: add downgrade option feature for pick-version
      }

      voke::argument_t &force_argument {voke::argument::find({"-f", "--force"})};

      std::string &local_version {local_library["version"]};
      std::string &host_version {host_version["version"]};

      VOKE_ASSERT(
        (
          force_argument == voke::argument::not_found
          &&
          local_version != "?"
          &&
          local_version == host_version
        ),
        voke::log() << "detail: library '" << sync_tag << "' version '" << local_library["version"] << "' already satisfied, keep using force option '-f | --force'",
        voke::result::SUCCESS
      );

      voke::argument_t &binary_argument {voke::argument::find({"-b", "--binary"})};
      if (binary_argument ==  voke::argument::not_found) {
        // @TODO: add binary option instead compile
        break;
      }

      bool was_cloned {};
      std::vector<voke::operation_t> operations {};
      std::unordered_map<std::string, bool> cloned_repositories {};

      for (voke::target_t &target : compiler_targets) {
        if (
          targets_argument != voke::argument::not_found
          &&
          std::find_if(
            targets_argument.values.begin(),
            targets_argument.values.end(),
            [target](std::string &value) { return value == target["tag"]; }
          ) == targets_argument.values.end()
        ) {
          continue;
        }

        std::string &target_url {target["url"]};
        if (!cloned_repositories[target_url]) {
          if (!target.count("git-clone-args")) {
            target["git-clone-args"] = "";
          }

          VOKE_ASSERT(
            voke::platform::sync_git_repository(
              target_url,
              host_library["cache-dir"],
              target["git-clone-args"]
            ),
            voke::log() << "fatal: could not sync git '" << target_url << '\'',
            voke::result::ERROR_FAILED
          );

          cloned_repositories[target_url] =  true;
        }

        VOKE_ASSERT(
          voke::platform::compile(
            host_library,
            target
          ),
          ekg::log() << "fatal: could not compile target '" << static_cast<std::string>(target["path"]) << '\'',
          voke::result::ERROR_FAILED
        );

        // @TODO: add post-build operations for this target
        operations.clear();
        VOKE_ASSERT(
          voke::platform::voke_system_fetch_library_target_operations(
            host_library,54
            target,
            operations
          ),
          ekg::log() << "error: could not obtain operations from target '" static_cast<std::string>(target["path"]) << '\'',
          voke::result::ERROR_FAILED
        );
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
