#include "os.hpp"
#include "io/resource.hpp"
#include "voke.hpp"
#include "io/log.hpp"

#include <iostream>
#include <filesystem>

voke::flags_t voke::platform::voke_system_fetch_compilers() {
  voke::flags_t result {voke::result::SUCCESS};

  std::ifstream installed_compilers(voke::platform::voke_system_installed_compilers_path);
  if (installed_compilers.is_open()) {
    std::string line {};
    size_t line_count {};
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
        voke::log() << "error: invalid 'installed-compilers.voke' syntax at line:\n " << line_count << " | " << line;
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

voke::flags_t voke::platform::voke_system_fetch_libraries() {
  voke::flags_t result {voke::result::SUCCESS};

  std::ifstream installed_libraries(voke::platform::voke_system_installed_libraries_path);
  if (installed_libraries.is_open()) {
    std::string line {};
    size_t line_count {};
    while (getline(installed_libraries, line)) {
      if (line.empty()) {
        continue;
      }

      voke::io::library_t library {};
      if (
        voke::io::extract_installed_library_info_from_line(
          line,
          library
        ) != voke::result::SUCCESS
      ) {
        voke::log() << "error: invalid 'installed-libraries.voke' syntax at line:\n " << line_count << " | " << line;
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
  std::ifstream installed_libraries(library.voke_path);
  if (installed_libraries.is_open()) {
    std::string line {};
    size_t line_count {};
    while (getline(installed_libraries, line)) {
      if (line.empty()) {
        continue;
      }

      if (
        voke::io::extract_library_info_from_line(
          line,
          library
        ) != voke::result::SUCCESS
      ) {
        voke::log() << "error: invalid '" <<  library.voke_path << "' syntax at line:\n " << line_count << " | " << line;
        return voke::result::WARNING_INVALID_LIBRARY;
      }

      line_count++;
    }

    installed_libraries.close();
  }

  voke::app.libraries.push_back(library);


  return voke::result::SUCCESS;
}


voke::flags_t voke::platform::voke_system_lookup_compilers_from_library(
  voke::io::library_t &library,
  std::vector<std::string> &compilers
) {
  for (const auto &entry : std::filesystem::directory_iterator(library.voke_path)) {
    std::string &compiler {
      compilers.emplace_back() = entry.path().string()
    };

    voke::log() << "detail: lookup result found voke-file '" << compiler << '"'; 
  }

  return voke::result::SUCCESS;
}
