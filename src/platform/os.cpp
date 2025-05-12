#include "os.hpp"
#include "io/log.hpp"
#include "io/shell.hpp"
#include "voke.hpp"
#include "io/vokefile.hpp"

#include <iostream>
#include <filesystem>

voke::flags_t voke::platform::voke_system_fetch_installed_compilers() {
  voke::log() << "detail: looking for installed compilers...";

  std::vector<std::string> lines {};
  voke::io::vokefile_read_lines(voke::system_installed_compilers_path, lines);

  voke::argument_compiler_info_t compiler_info {
    .tag = "installed-compilers.voke",
    .lines = lines, 
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
    voke::empty,
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
    voke::empty,
    voke::result::ERROR_FAILED
  );

  voke::log() << "detail: checked " << voke::app.installed_compilers.size() << " compilers installed";
  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::voke_system_fetch_installed_libraries() {
  voke::log() << "detail: looking for installed libraries...";

  std::vector<std::string> lines {};
  voke::io::vokefile_read_lines(voke::system_installed_libraries_path, lines);

  voke::argument_compiler_info_t compiler_info {
    .tag = "installed-libraries.voke",
    .lines = lines,
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
    voke::empty,
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
    voke::empty,
    voke::result::ERROR_FAILED
  );

  voke::log() << "detail: checked " << voke::app.installed_compilers.size() << " libraries installed";
  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::voke_system_fetch_library_target(
  voke::library_t &library,
  voke::target_t &target
) {
  voke::log() << "detail: fetching library target...";

  std::vector<std::string> lines {};
  voke::io::vokefile_read_lines(library["path"], lines);

  voke::argument_compiler_info_t compiler_info {
    .tag = static_cast<std::string&>(library["tag"]),
    .lines = lines,
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
    voke::empty,
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
    voke::empty,
    voke::result::ERROR_FAILED
  );

  std::string &url {static_cast<std::string&>(target["url"])};
  std::string end {};

  voke::io::cut_end_of_url(url, end);
  target["cache-dir"] = voke::system_cached_repositories_dir_path + end;

  voke::log() << "detail: library target '" << end << "' fetched";
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
      voke::empty,
      voke::result::ERROR_FAILED
    );

    VOKE_ASSERT(
      voke::resource::pack<voke::operation_t>(
        pack_info,
        operations.emplace_back()
      ),
      voke::empty,
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
  voke::log() << "detail: building library'" << static_cast<std::string&>(library["tag"]) << "'...";

  std::string &cache_dir {static_cast<std::string&>(library["cache-dir"])};
  std::string &run {static_cast<std::string&>(library["run"])};
  std::string &cpp {static_cast<std::string&>(library["cpp"])};
  std::string &c {static_cast<std::string&>(library["c"])};
  std::string &compiler_binary_dir {static_cast<std::string&>(library["binary-dir"])};

  voke::io::replace(
    run,
    "\\$dir",
    cache_dir
  );

  voke::io::replace(
    run,
    "\\$cpp",
    compiler_binary_dir + "/" + cpp
  );

  voke::io::replace(
    run,
    "\\$c",
    compiler_binary_dir + "/" + c
  );

  voke::io::replace(
    run,
    "$cmake-build-dir",
    "./cmake-build"
  );

  voke::shell()
    << "cd "
    << cache_dir
    << " && "
    << run;

  voke::flags_t result {
    voke::shell::result
    ==
    0
    ? voke::result::SUCCESS : voke::result::ERROR_FAILED
  };

  if (
    result == voke::result::SUCCESS
    &&
    !cache_dir.empty()
    &&
    static_cast<std::string&>(library["build_system"]) == "cmake"
  ) {
    voke::shell()
      << "sudo rm -r "
      << cache_dir
      << "/cmake-build";
  }

  voke::log() << "detail: building done";
}
