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
  voke::io::vokefile_read_lines(voke::platform::voke_system_installed_compilers_path, lines);

  voke::argument_compiler_info_t compiler_info {
    .tag = "installed-compilers.voke",
    .lines = lines, 
    .expect = {
      {{"--tag"}, 1, voke::mandatory},
      {{"--binary-dir"}, 1, voke::mandatory},
      {{"--lib-dir"}, 1, voke::mandatory},
      {{"--include-dir"}, 1, voke::mandatory},
      {{"--c"}, 1, voke::mandatory},
      {{"--cpp"}, 1, voke::mandatory},
      {{"--version"}, 1, voke::mandatory},
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

  size_t line {1};
  size_t size {args.size()};

  voke::compiler_t compiler {};
  std::vector<voke::compiler_t> unchecked_compilers {};

  for (size_t it {}; it < size; it++) {
    voke::argument_t &argument {
      args.at(it)
    };

    if (argument.prefix.empty()) {
      continue;
    }

    if (argument.line != line) {
      unchecked_compilers.push_back(compiler);
      compiler = {};
      line = argument.line;
    }

    compiler[argument.prefix] = argument;

    if (it == size - 1) {
      unchecked_compilers.push_back(compiler);
    }
  }

  std::vector<std::string> mandatory {
    {"--tag"},
    {"--binary-dir"},
    {"--lib-dir"},
    {"--include-dir"},
    {"--c"},
    {"--cpp"},
    {"--version"}
  };

  size_t errors {};
  for (voke::compiler_t &compiler : unchecked_compilers) {
    errors = voke::resource::mandatory<voke::compiler_t>(
      mandatory,
      compiler
    );

    if (errors == 0) {
      voke::app.installed_compilers.push_back(compiler);
      voke::log() << "detail: " << compiler["tag"] << ' ' << compiler["version"];
    } else {
      voke::log() << "fatal: " << errors << " error(s) occured, failed to recognise compiler '" << compiler["tag"] << '\'';
    }
  }

  return result;
}

voke::flags_t voke::platform::voke_system_fetch_installed_libraries() {
  voke::log() << "detail: looking for installed libraries...";
  voke::flags_t result {voke::result::SUCCESS};

  std::vector<std::string> lines {};
  voke::io::vokefile_read_lines(voke::platform::voke_system_installed_libraries_path, lines);

  voke::argument_compiler_info_t compiler_info {
    .tag = "installed-libraries.voke",
    .lines = lines,
    .expect = {
      {{"--tag"}, voke::not_empty, voke::mandatory},
      {{"--headers"}, voke::any, voke::optional},
      {{"--headers-dir"}, voke::any, voke::optional},
      {{"--libraries"}, voke::any, voke::optional},
      {{"--libraries-dir"}, voke::any, voke::optional},
      {{"--targets"}, voke::not_empty, voke::mandatory},
      {{"--version"}, 1, voke::mandatory},
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

  size_t line {1};
  size_t size {args.size()};

  voke::library_t library {};
  std::vector<voke::library_t> unchecked_libraries {};

  for (size_t it {}; it < size; it++) {
    voke::argument_t &argument {
      args.at(it)
    };

    if (argument.prefix.empty()) {
      continue;
    }

    if (argument.line != line) {
      unchecked_libraries.push_back(library);
      library = {};
      line = argument.line;
    }

    library[argument.prefix] = argument;

    if (it == size - 1) {
      unchecked_libraries.push_back(library);
    }
  }

  std::vector<std::string> mandatory {
    {"--tag"},
    {"--targets"},
    {"--version"},
    {"--binaries", "--binaries-dir"},
    {"--headers", "--headers-dir"},
  };

  size_t errors {};
  for (voke::library_t &library : unchecked_compilers) {
    errors = voke::resource::mandatory<voke::library_t>(
      mandatory,
      library
    );

    if (errors == 0) {
      voke::app.installed_compilers.push_back(library);
      voke::log() << "detail: " << library["tag"] << ' ' << library["version"];
    } else {
      voke::log() << "fatal: " << errors << " error(s) occured, failed to recognise library '" << library["tag"] << '\'';
    }
  }

  return result;
}

voke::flags_t voke::platform::voke_system_fetch_sync_library_target(
  voke::library_t &library
) {
  voke::flags_t result {voke::result::SUCCESS};
  
  std::vector<std::string> lines {};
  voke::io::vokefile_read_lines(library.voke_path, lines);

  voke::argument_compiler_info_t compiler_info {
    .tag = library.voke_tag,
    .lines = lines, 
    .expect = {
      {{"--tag"}, voke::not_empty, voke::mandatory},
      {{"--build-system"}, voke::not_empty, voke::mandatory},
      {{"--url"}, voke::not_empty, voke::mandatory},
      {{"--git-clone-args"}, voke::not_empty, voke::optional},
      {{"--targets"}, voke::not_empty, voke::mandatory},
      {{"--headers"}, voke::not_empty, voke::mandatory},
      {{"--include-dirs"}, voke::not_empty, voke::mandatory},
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

  if (args.empty()) {
    result = voke::result::ERROR_FAILED;
    return result;
  }

  size_t line {};
  for (voke::argument_t &argument : args) {
    library.error_counter += voke::resource::serialize_library_from_argument(
      argument,
      library
    );
  }

  if (library.error_counter == 0) {
    voke::app.libraries.push_back(library);
    voke::log() << "detail: " << library.tag << ' ' << library.version;
  } else {
    voke::log() << "error: " << library.error_counter << " error(s) occured, failed to recognise library '" << library.tag << '\'';
  }

  return result;
}

voke::flags_t voke::platform::voke_system_lookup_compilers_from_host_library(
  voke::library_t &library,
  std::vector<std::string> &compilers
) {
  for (const auto &entry : std::filesystem::directory_iterator(library.voke_path)) {
    std::string &compiler {
      compilers.emplace_back() = entry.path().string()
    };

    voke::log() << "detail: found voke-file at '" << compiler << '"'; 
  }

  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::voke_system_fetch_compilers_info_from_host_library(
  voke::library_t &library,
  std::vector<std::string> &compilers
) {
  for (std::string &compiler_path : compilers) {
    library.voke_path = compiler_path;
    voke::platform::voke_system_fetch_sync_library_target(
      library
    );
  };

  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::compile_libraries(
) {
  voke::result result {};
  for (voke::library_t &library : voke::app.libraries) {
    if (library.targets.empty()) {
      voke::log() << "warning: skipping library voke-file '" << library.voke_path << "', no C/C++ compiler target specified";
      continue;
    }

    result = voke::result::SUCCESS_PASS;
    for (voke::compiler_t &compiler : voke::app.installed_compilers) {
      for (std::string &target : library.targets) {
        if (target == compiler["tag"]) {
          result = voke::result::SUCCESS;
          break;
        }
      }

      if (result == voke::result::SUCCESS) {
        result = voke::result::SUCCESS_PASS;

        voke::log() << "detail: building library '" << library.voke_tag << "' for C/C++ target '" << compiler["tag"] << "'...";
        
        voke::io::replace(
          library.run,
          "\\$dir",
          library.repository_cache_path
        );

        voke::io::replace(
          library.run,
          "\\$cpp",
          compiler.binary_dir + "/" + compiler.cpp_compiler
        );

        voke::io::replace(
          library.run,
          "\\$c",
          compiler.binary_dir + "/" + compiler.c_compiler
        );

        voke::io::replace(
          library.run,
          "$cmake-build-dir",
          "./cmake-build"
        );

        voke::shell()
          << "cd "
          << library.repository_cache_path
          << " && "
          << library.run;

        result = (
          voke::shell::result
          ==
          0
          ? voke::result::SUCCESS : voke::result::ERROR_FAILED
        );

        if (
          result == voke::result::SUCCESS
          &&
          !library.repository_cache_path.empty()
          &&
          library.build_system == "cmake"
        ) {
          voke::shell()
            << "sudo rm -r "
            << library.repository_cache_path
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
      voke::log() << "detail: library '" << library.voke_tag << "' successfully build";
      break;
    case voke::result::ERROR_FAILED:
      voke::log() << "detail: could not build library '" << library.voke_tag << "'";
      break;
    default:
      break;
    }
  }   

  return voke::result::SUCCESS;
}
