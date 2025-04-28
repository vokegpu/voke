#include "os.hpp"
#include "io/resource.hpp"
#include "voke.hpp"
#include "io/log.hpp"

#include <iostream>
#include <filesystem>

voke::shell_result_t voke::shell::result {};

voke::flags_t voke::platform::voke_system_fetch_installed_compilers() {
  voke::log() << "detail: looking for installed compilers...";
  voke::flags_t result {voke::result::SUCCESS};
  std::ifstream installed_compilers(voke::platform::voke_system_installed_compilers_path);
  if (installed_compilers.is_open()) {
    std::string line {};
    size_t line_count {1};
    while (getline(installed_compilers, line)) {
      if (line.empty()) {
        continue;
      }

      voke::io::compiler_t compiler {};
      if (
        voke::io::extract_installed_compiler_info_from_line(
          line,
          compiler
        ) != voke::result::SUCCESS
      ) {
        voke::log() << "error: invalid '" << voke::log::error << "' in 'installed-compilers.voke' syntax at line:\n " << line_count << " | " << line;
        result = voke::result::WARNING_INVALID_COMPILER;
        line_count++;
        continue;
      }

      voke::app.installed_compilers.push_back(compiler);
      voke::log() << "detail: " << compiler.tag << " version " << compiler.version;
      line_count++;
    }

    installed_compilers.close();
  }

  return result;
}

voke::flags_t voke::platform::voke_system_fetch_installed_libraries() {
  voke::flags_t result {voke::result::SUCCESS};
  std::ifstream installed_libraries(voke::platform::voke_system_installed_libraries_path);
  if (installed_libraries.is_open()) {
    std::string line {};
    size_t line_count {1};
    size_t pos {};
    while (getline(installed_libraries, line)) {
      if (line.empty()) {
        continue;
      }

      if ((pos = line.find('\n')) != std::string::npos) {
        line.erase(line.begin() + pos);
      }

      if ((pos = line.find('\r')) != std::string::npos) {
        line.erase(line.begin() + pos);
      }

      voke::io::library_t library {};
      if (
        voke::io::extract_installed_library_info_from_line(
          line,
          library
        ) != voke::result::SUCCESS
      ) {
        voke::log() << "error: invalid '" << voke::log::error << "' in 'installed-libraries.voke' syntax at line:\n " << line_count << " | " << line;
        result = voke::result::WARNING_INVALID_LIBRARY;
        line_count++;
        continue;
      }

      voke::app.installed_libraries.push_back(library);
      voke::log() << "detail: " << library.tag << " version " << library.version;
      line_count++;
    }

    installed_libraries.close();
  }

  return result;
}

voke::flags_t voke::platform::voke_system_fetch_library(
  voke::io::library_t &library
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
  voke::io::library_t &library,
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
  voke::io::library_t &library,
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
  for (voke::io::library_t &library : voke::app.libraries) {
    if (library.target.empty()) {
      voke::log() << "warning: skipping library voke-file '" << library.voke_path << "', no C/C++ compiler target specified";
      continue;
    }

    result = voke::result::SUCCESS_PASS;
    for (voke::io::compiler_t &compiler : voke::app.installed_compilers) {
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
