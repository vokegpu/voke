#include "os.hpp"
#include "io/log.hpp"
#include "io/shell.hpp"
#include "voke.hpp"
#include "io/vokefile.hpp"

#include <iostream>
#include <filesystem>
#include <functional>

std::string voke::system_dir_path {};
std::string voke::system_installed_compilers_path {};
std::string voke::system_installed_libraries_path {};
std::string voke::system_cached_repositories_dir_path {};

voke::flags_t voke::platform::voke_system_init() {
  std::string home {};

  #if defined(_WIN32) || defined(_WIN64)
    // add windows path
  #elif defined(__linux__)
    voke::shell() << "cd ~/.voke";
    if (voke::shell::result != 0) {
      voke::shell() << "mkdir -p .voke/cache";
      voke::shell() << "touch " << voke::system_installed_compilers_path;
      voke::shell() << "touch " << voke::system_installed_libraries_path;

      // @TODO: add auto-fetch targets
      // @TODO: add auto-fetch libraries (how)
    }

    std::string command_builder {"echo $HOME > "};
    size_t hash {std::hash<std::string>{}("home")};
    command_builder += std::to_string(hash);
    std::system(command_builder.c_str());

    std::vector<std::string> lines {};
    voke::io::vokefile_read_lines(std::to_string(hash), lines);

    command_builder = "rm ";
    command_builder += std::to_string(hash);
    std::system(command_builder.c_str());

    if (lines.empty()) {
      return voke::result::ERROR_FAILED;
    }

    voke::system_dir_path = lines.at(0);
    voke::system_dir_path += "/.voke/system/";

    voke::system_cached_repositories_dir_path = lines.at(0);
    voke::system_cached_repositories_dir_path += "/.voke/cache/";

    voke::system_installed_compilers_path = lines.at(0);
    voke::system_installed_compilers_path += "/.voke/installed-compilers.voke";

    voke::system_installed_libraries_path = lines.at(0);
    voke::system_installed_libraries_path += "/.voke/installed-libraries.voke";
  #endif

  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::voke_system_fetch_installed_compilers() {
  voke::log(voke::verbose_level::LEVEL_TWO) << "detail: looking for installed compilers...";

  std::vector<std::string> lines {};
  VOKE_ASSERT(
    voke::io::vokefile_read_lines(voke::system_installed_compilers_path, lines),
    voke::log() << "fatal: could not read '" << voke::system_installed_compilers_path << '\'',
    voke::result::ERROR_FAILED
  );

  voke::argument_compiler_info_t compiler_info {
    .tag = "installed-compilers.voke",
    .lines = lines,
    .allow_repeated_arguments = true,
    .expect = {
      {{"--tag"}, 1, voke::behavior::MANDATORY},
      {{"--binary-dir"}, 1, voke::behavior::MANDATORY},
      {{"--lib-dir"}, 1, voke::behavior::MANDATORY},
      {{"--include-dir"}, 1, voke::behavior::MANDATORY},
      {{"--c"}, 1, voke::behavior::MANDATORY},
      {{"--cpp"}, 1, voke::behavior::MANDATORY},
      {{"--version"}, 1, voke::behavior::MANDATORY},
    }
  };

  std::vector<voke::argument_t> args {};
  VOKE_ASSERT(
    voke::argument::compile(
      compiler_info,
      args
    ),
    /* empty */,
    voke::result::ERROR_FAILED
  );

  voke::resource_pack_info_t pack_info {
    .tag = "installed-compilers",
    .type = voke::host,
    .compiled_arguments = args
  };

  VOKE_ASSERT(
    voke::resource::packs<voke::compiler_t>(
      pack_info,
      voke::app.installed_compilers
    ),
    /* empty */,
    voke::result::ERROR_FAILED
  );

  voke::log(voke::verbose_level::LEVEL_TWO) << "detail: checked " << voke::app.installed_compilers.size() << " compilers installed";
  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::voke_system_fetch_installed_libraries() {
  voke::log(voke::verbose_level::LEVEL_TWO) << "detail: looking for installed libraries...";

  std::vector<std::string> lines {};
  voke::io::vokefile_read_lines(voke::system_installed_libraries_path, lines);

  voke::argument_compiler_info_t compiler_info {
    .tag = "installed-libraries.voke",
    .lines = lines,
    .allow_repeated_arguments = true,
    .expect = {
      {{"--tag"}, voke::not_empty, voke::behavior::MANDATORY},
      {{"--headers"}, voke::any, voke::behavior::OPTIONAL},
      {{"--headers-dir"}, voke::any, voke::behavior::OPTIONAL},
      {{"--libraries"}, voke::any, voke::behavior::OPTIONAL},
      {{"--libraries-dir"}, voke::any, voke::behavior::OPTIONAL},
      {{"--targets"}, voke::not_empty, voke::behavior::MANDATORY},
      {{"--version"}, 1, voke::behavior::MANDATORY},
    }
  };

  std::vector<voke::argument_t> args {};
  VOKE_ASSERT(
    voke::argument::compile(
      compiler_info,
      args
    ),
    /* empty */,
    voke::result::ERROR_FAILED
  );

  voke::resource_pack_info_t pack_info {
    .tag = "installed-libraries",
    .type = voke::host,
    .compiled_arguments = args
  };

  VOKE_ASSERT(
    voke::resource::packs<voke::library_t>(
      pack_info,
      voke::app.installed_libraries
    ),
    /* empty */,
    voke::result::ERROR_FAILED
  );

  voke::log(voke::verbose_level::LEVEL_TWO) << "detail: checked " << voke::app.installed_libraries.size() << " libraries installed";
  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::voke_system_fetch_library_target(
  voke::library_t &library,
  voke::target_t &target
) {
  voke::log() << "detail: fetching library target...";

  std::vector<std::string> lines {};
  voke::io::vokefile_read_lines(target["path"], lines);

  std::string compacted_line {};
  for (std::string &line : lines) {
    compacted_line += line;
    compacted_line += ' ';
  }

  lines.clear();
  lines.emplace_back() = compacted_line;

  voke::argument_compiler_info_t compiler_info {
    .tag = static_cast<std::string&>(library["tag"]),
    .lines = lines,
    .allow_repeated_arguments = true,
    .expect = {
      {{"--tag"}, voke::not_empty, voke::behavior::MANDATORY},
      {{"--build-system"}, voke::not_empty, voke::behavior::MANDATORY},
      {{"--url"}, voke::not_empty, voke::behavior::MANDATORY},
      {{"--git-clone-args"}, voke::not_empty, voke::behavior::OPTIONAL},
      {{"--operation"}, 1, voke::behavior::MANDATORY},
      {{"--run"}, voke::not_empty, voke::behavior::MANDATORY},
    }
  };
  
  std::vector<voke::argument_t> args {};
  VOKE_ASSERT(
    voke::argument::compile(
      compiler_info,
      args
    ),
    /* empty */,
    voke::result::ERROR_FAILED
  );

  voke::resource_pack_info_t pack_info {
    .tag = static_cast<std::string&>(library["tag"]),
    .type = voke::local,
    .compiled_arguments = args
  };

  VOKE_ASSERT(
    voke::resource::pack<voke::target_t>(
      pack_info,
      target
    ),
    /* empty */,
    voke::result::ERROR_FAILED
  );

  target["cache-dir"] = voke::system_cached_repositories_dir_path + static_cast<std::string&>(library["tag"]);
  library["cache-dir"] = target["cache-dir"];

  voke::log() << "detail: library target '" << static_cast<std::string&>(target["cache-dir"]) << "' fetched";
  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::voke_system_fetch_library_target_operations(
  voke::library_t &library,
  voke::target_t &target,
  std::vector<voke::operation_t> &operations
) {
  voke::resource_pack_info_t pack_info { .tag = static_cast<std::string&>(library["tag"]), .type = voke::host };
  voke::argument_compiler_info_t compiler_info {
    .tag = static_cast<std::string&>(library["tag"]),
    .expect = {
      {{"--tag"}, 1, voke::behavior::MANDATORY},
      {{"--type"}, 1, voke::behavior::MANDATORY},
      {{"--files"}, voke::not_empty, voke::behavior::MANDATORY},
      {{"--targets"}, voke::not_empty, voke::behavior::OPTIONAL},
    }
  };

  for (auto &[key, value] : target) {
    if (key.find("operations") == std::string::npos) {
      continue;
    }

    pack_info.compiled_arguments.clear();
    VOKE_ASSERT(
      voke::argument::compile(
        compiler_info,
        pack_info.compiled_arguments
      ),
      /* empty */,
      voke::result::ERROR_FAILED
    );

    VOKE_ASSERT(
      voke::resource::pack<voke::operation_t>(
        pack_info,
        operations.emplace_back()
      ),
      /* empty */,
      voke::result::ERROR_FAILED
    );
  }

  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::voke_system_compile_host_library(
  voke::library_t &library,
  voke::target_t &target,
  std::vector<voke::operation_t> &operations,
  voke::compiler_t &compiler
) {
  voke::log() << "detail: building library '" << static_cast<std::string&>(library["tag"]) << "'...";

  std::string &cache_dir {static_cast<std::string&>(library["cache-dir"])};
  std::string &run {static_cast<std::string&>(target["run"])};
  std::string &cpp {static_cast<std::string&>(compiler["cpp"])};
  std::string &c {static_cast<std::string&>(compiler["c"])};
  std::string &compiler_binary_dir {static_cast<std::string&>(compiler["binary-dir"])};
  std::string cmake_build_dir {};

  voke::io::replace(
    run,
    "\\$\\{dir\\}",
    cache_dir
  );

  voke::io::replace(
    run,
    "\\$\\{cpp\\}",
    compiler_binary_dir + "/" + cpp
  );

  voke::io::replace(
    run,
    "\\$\\{c\\}",
    compiler_binary_dir + "/" + c
  );

  cmake_build_dir += cache_dir;
  cmake_build_dir += "/cmake-build";
  voke::io::replace(
    run,
    "\\$\\{cmake-build-dir\\}",
    cmake_build_dir
  );

  voke::log() << '\n';

  voke::shell(true)
    << "cd "
    << cache_dir
    << " && "
    << run;

  voke::log() << '\n';

  VOKE_ASSERT(
    voke::shell::result,
    voke::log() << "error: could not build library",
    voke::result::ERROR_FAILED
  );

  if (
    !cache_dir.empty()
    &&
    static_cast<std::string&>(target["build-system"]) == "cmake"
    &&
    false
  ) {
    voke::shell()
      << "sudo rm -r "
      << cache_dir
      << "/cmake-build";
  }

  voke::log() << "detail: building done";
  return voke::result::SUCCESS;
}
