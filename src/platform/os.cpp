#include "resource/compiler.hpp"
#include "resource/library.hpp"
#include "os.hpp"
#include "io/log.hpp"
#include "voke.hpp"
#include "io/vokefile.hpp"

#include <iostream>
#include <filesystem>

voke::shell_result_t voke::shell::result {};

voke::flags_t voke::platform::voke_system_fetch_installed_compilers() {
  voke::log() << "detail: looking for installed compilers...";
  voke::flags_t result {voke::result::SUCCESS};

  voke::vokefile_parser_info parser_info {
    .tag = "installed-compilers.voke",
    .path = voke::platform::voke_system_installed_compilers_path
    .expect = {
      {{"--tag"}, 1, true},
      {{"--binary-dir"}, 1, true},
      {{"--lib-dir"}, 1, true},
      {{"--include-dir"}, 1, true},
      {{"--c"}, 1, true},
      {{"--cpp"}, 1, true},
      {{"--version"}, 1, true},
    }
  };

  voke::argument_list_t args {};
  voke::vokefile_parser_to_args(
    parser_info,
    args
  );

  voke::compiler_t compiler {};
  voke::argument_list_t compiler_args(1);
  size_t line {1};

  for (voke::argument_t &argument : args) {
    if (argument.line != line) {
      if (compiler.error_counter == 0) {
        voke::app.installed_compilers.push_back(compiler);
        voke::log() << "detail: " << compiler.tag << ' ' << compiler.version;
      } else {
        voke::log() << "error: " << compiler.error_counter << " error(s) occured, failed to recognise compiler '" << compiler.tag << "' at line " << compiler.line;
      }

      compiler = {};
      line = argument.line;
    }

    compiler_args.at(0) = argument;
    compiler.error_counter += voke::resource::serialize_compiler_from_argument_list(
      compiler_args.at(0),
      compiler
    );
  }

  if (!args.empty()) {
    if (compiler.error_counter == 0) {
      voke::app.installed_compilers.push_back(compiler);
      voke::log() << "detail: " << compiler.tag << ' ' << compiler.version;
    } else {
      voke::log() << "error: " << compiler.error_counter << " error(s) occured, failed to recognise compiler '" << compiler.tag << "' at line " << compiler.line;
    }
  }

  return result;
}

voke::flags_t voke::platform::voke_system_fetch_installed_libraries() {
  voke::log() << "detail: looking for installed libraries...";
  voke::flags_t result {voke::result::SUCCESS};

  voke::vokefile_parser_info parser_info {
    .tag = "installed-libraries.voke",
    .path = voke::platform::voke_system_installed_libraries_path
    .expect = {
      {{"--tag"}, voke::not_empty, true},
      {{"--headers"}, voke::any, false},
      {{"--headers-dir"}, voke::any, false},
      {{"--libraries"}, voke::any, false},
      {{"--libraries-dir"}, voke::any, false},
      {{"--targets"}, voke::not_empty, true},
      {{"--version"}, 1, true},
    }
  };

  voke::argument_list_t args {};
  voke::vokefile_parser_to_args(
    parser_info,
    args
  );

  voke::library_t library {};
  voke::argument_list_t library_args(1);
  size_t line {};

  for (voke::argument_t &argument : args) {
    if (argument.line != line) {
      if (library.error_counter == 0) {
        voke::app.installed_libraries.push_back(library);
        voke::log() << "detail: " << library.tag << ' ' << library.version;
      } else {
        voke::log() << "error: " << library.error_counter << " error(s) occured, failed to recognise library '" << library.tag << "' at line " << compiler.line;
      }

      voke::app.installed_libraries.push_back(library);
      library = {};
      line = argument.line;
    }

    library_args.at(0) = argument;
    library.error_counter += voke::resource::serialize_library_from_argument_list(
      library_args.at(0),
      library
    );
  }

  if (!args.empty()) {
    if (library.error_counter == 0) {
      voke::app.installed_compilers.push_back(library);
      voke::log() << "detail: " << library.tag << ' ' << library.version;
    } else {
      voke::log() << "error: " << library.error_counter << " error(s) occured, failed to recognise library '" << library.tag << "' at line " << compiler.line;
    }
  }

  return result;
}

voke::flags_t voke::platform::voke_system_fetch_library(
  voke::library_t &library
) {
  voke::log() << "detail: loading voke-file at '" << library.voke_path << "'...";
  std::ifstream voke_library(library.voke_path);
  if (voke_library.is_open()) {
    std::string line {};
    size_t line_count {1};
    size_t pos {};
    while (getline(voke_library, line)) {
      if (line.empty()) {
        continue;
      }

      if ((pos = line.find('\n')) != std::string::npos) {
        line.erase(line.begin() + pos);
      }

      if ((pos = line.find('\r')) != std::string::npos) {
        line.erase(line.begin() + pos);
      }

      if (
        voke::io::extract_library_info_from_line(
          line,
          library
        ) != voke::result::SUCCESS
      ) {
        voke::log() << "error: invalid '" << voke::log::error << "' in '" <<  library.voke_path << "' syntax at line:\n " << line_count << " | " << line;
        return voke::result::WARNING_INVALID_LIBRARY;
      }

      line_count++;
    }

    voke_library.close();
  }

  voke::log() << "detail: voke-file at '" << library.voke_path << "' loaded";
  voke::app.libraries.push_back(library);
  return voke::result::SUCCESS;
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
    voke::platform::voke_system_fetch_library(
      library
    );
  };

  return voke::result::SUCCESS;
}

voke::flags_t voke::platform::compile_libraries(
) {
  voke::result result {};
  for (voke::library_t &library : voke::app.libraries) {
    if (library.target.empty()) {
      voke::log() << "warning: skipping library voke-file '" << library.voke_path << "', no C/C++ compiler target specified";
      continue;
    }

    result = voke::result::SUCCESS_PASS;
    for (voke::compiler_t &compiler : voke::app.installed_compilers) {
      for (std::string &target : library.target) {
        if (target == compiler.tag) {
          result = voke::result::SUCCESS;
          break;
        }
      }

      if (result == voke::result::SUCCESS) {
        result = voke::result::SUCCESS_PASS;

        voke::log() << "detail: building library '" << library.voke_tag << "' for C/C++ target '" << compiler.tag << "'...";
        
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
