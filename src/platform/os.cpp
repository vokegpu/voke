#include "os.hpp"
#include "io/log.hpp"
#include "io/shell.hpp"
#include "voke.hpp"
#include "io/vokefile.hpp"

#include <iostream>
#include <filesystem>

voke::flags_t voke::platform::voke_system_fetch_installed_compilers() {
  voke::log() << "detail: looking for installed compilers...";
  voke::flags_t result {voke::result::SUCCESS};

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
  if (
      voke::argument::compile(
      compiler_info,
      args
    ) != voke::result::SUCCESS
  ) {
    return voke::result::ERROR_FAILED;
  }

  voke::resource_pack_info_t pack_info {
    .tag = "installed-compilers",
    .type = voke::host,
    .compiled_arguments = args
  };

  return voke::resource::packs<voke::compiler_t>(
    pack_info,
    voke::app.installed_compilers
  );
}

voke::flags_t voke::platform::voke_system_fetch_installed_libraries() {
  voke::log() << "detail: looking for installed libraries...";
  voke::flags_t result {voke::result::SUCCESS};

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
  if (
      voke::argument::compile(
      compiler_info,
      args
    ) != voke::result::SUCCESS
  ) {
    return voke::result::ERROR_FAILED;
  }

  voke::resource_pack_info_t pack_info {
    .tag = "installed-libraries",
    .type = voke::host,
    .compiled_arguments = args
  };

  return voke::resource::packs<voke::library_t>(
    pack_info,
    voke::app.installed_libraries
  );
}

voke::flags_t voke::platform::voke_system_fetch_target(
  voke::library_t &library,
  voke::target_t &target
) {
  voke::flags_t result {voke::result::SUCCESS};
  
  std::vector<std::string> lines {};
  voke::io::vokefile_read_lines(library["path"], lines);

  voke::argument_compiler_info_t compiler_info {
    .tag = library["tag"],
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
  if (
      voke::argument::compile(
        compiler_info,
        args
      ) != voke::result::SUCCESS
    ) {
    return voke::result::ERROR_FAILED;
  }

  voke::resource_pack_info_t pack_info {
    .tag = library["tag"],
    .type = voke::local,
    .compiled_arguments = args
  };

  return voke::argument::pack<voke::target_t>(
    pack_info,
    target
  );
}

voke::flags_t voke::platform::voke_system_fetch_library_target_operations(
  voke::library_t &library,
  voke::targe_t &target,
  std::vector<voke::operation_t> &operations
) {
  voke::resource_pack_info_t pack_info { .tag = library["tag"], .type = voke::host };
  voke::argument_compiler_info_t compiler_info {
    .tag = library["tag"],
    .expect = {
      {{"--tag"}, 1, voke::behavior::MANDATORY},
      {{"--type"}, 1, voke::behavior::MANDATORY},
      {{"--files"}, voke::not_empty, voke::behavior::MANDATORY},
      {{"--targets"}, voke::not_empty, voke::behavior::OPTIONAL},
    }
  };

  for (&[key, value] : target) {
    if (key.find("operations") == std::string::npos) {
      continue;
    }

    pack_info.compiled_arguments.clear();
    if (
      voke::argument::compile(
        compiler_info,
        pack_info.compiled_arguments
      ) != voke::result::SUCCESS
    ) {
      return voke::result::ERROR_FAILED;
    }

    if (
      voke::resource::pack(
        pack_info,
        operations.emplace_back()
      ) != voke::result::SUCCESS
    ) {
      return voke::result::ERROR_FAILED;
    }
  }

  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::compile_libraries() {
  voke::result result {};
  for (voke::library_t &library : voke::app.libraries) {
    if (library["targets"].empty()) {
      voke::log()
        << "warning: skipping library voke-file '"
        << static_cast<std::string>(library["path"])
        << "', no C/C++ compiler target specified";
      continue;
    }

    result = voke::result::SUCCESS_PASS;
    for (voke::compiler_t &compiler : voke::app.installed_compilers) {
      for (std::string &target : library["targets"]) {
        if (target == compiler["tag"]) {
          result = voke::result::SUCCESS;
          break;
        }
      }

      if (result == voke::result::SUCCESS) {
        result = voke::result::SUCCESS_PASS;

        voke::log()
          << "detail: building library '"
          << static_cast<std::string>(library["tag"])
          << "' for C/C++ target '"
          << compiler["tag"] << "'...";
        
        voke::io::replace(
          library["run"],
          "\\$dir",
          library["repository_cache_path"]
        );

        voke::io::replace(
          library["run"],
          "\\$cpp",
          compiler["binary_dir"] + "/" + compiler["cpp_compiler"]
        );

        voke::io::replace(
          library["run"],
          "\\$c",
          compiler["binary_dir"] + "/" + compiler["c_compiler"]
        );

        voke::io::replace(
          library["run"],
          "$cmake-build-dir",
          "./cmake-build"
        );

        voke::shell()
          << "cd "
          << library["repository_cache_path"]
          << " && "
          << library["run"];

        result = (
          voke::shell::result
          ==
          0
          ? voke::result::SUCCESS : voke::result::ERROR_FAILED
        );

        if (
          result == voke::result::SUCCESS
          &&
          !library["repository_cache_path"].empty()
          &&
          library["build_system"] == "cmake"
        ) {
          voke::shell()
            << "sudo rm -r "
            << library["repository_cache_path"]
            << "/cmake-build";
        }

        break;
      }
    }

    switch (result) {
    case voke::result::SUCCESS_PASS:
      voke::log() << "warning: nothing happened actually";
      break;
    case voke::result::SUCCESS:
      voke::log()
        << "detail: library '"
        << static_cast<std::string>(library["tag"])
        << "' successfully build";
      break;
    case voke::result::ERROR_FAILED:
      voke::log()
        << "detail: could not build library '"
        << static_cast<std::string>(library["tag"]) << "'";
      break;
    default:
      break;
    }
  }   

  return voke::result::SUCCESS;
}
