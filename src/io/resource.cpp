#include "resource.hpp"
#include "args.hpp"
#include "log.hpp"

#include <algorithm>

voke::flags_t voke::io::extract_installed_compiler_info_from_line(
  std::string &line,
  voke::io::compiler_t &compiler
) {
  std::vector<std::string> splitted {voke::io::split(line, ' ')};
  splitted.insert(splitted.begin(), "meow ");

  std::vector<char*> splitted_cstr {};
  for (std::string &raw_arg : splitted) {
    splitted_cstr.push_back(raw_arg.data());
  }

  std::vector<voke::io::argument_t> args {};
  voke::io::fill(splitted_cstr.size(), splitted_cstr.data(), args);

  for (voke::io::argument_t &arg : args) {
    if (arg.prefix == "--tag") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      compiler.tag = arg.values.at(0);
    }

    if (arg.prefix == "--binary-dir") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      compiler.binary_dir = arg.values.at(0);
    }

    if (arg.prefix == "--lib-dir") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      compiler.lib_dir = arg.values.at(0);
    }

    if (arg.prefix == "--include-dir") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      compiler.include_dir = arg.values.at(0);
    }

    if (arg.prefix == "--c") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      compiler.c_compiler = arg.values.at(0);
    }

    if (arg.prefix == "--cpp") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      compiler.cpp_compiler = arg.values.at(0);
    }

    if (arg.prefix == "--version") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      compiler.version = arg.values.at(0);
    }
  }

  return voke::result::SUCCESS;
}

voke::flags_t voke::io::extract_installed_library_info_from_line(
  std::string &line,
  voke::io::library_t &library
) {
  std::vector<std::string> splitted {voke::io::split(line, ' ')};
  splitted.insert(splitted.begin(), "meow ");

  std::vector<char*> splitted_cstr {};
  for (std::string &raw_arg : splitted) {
    splitted_cstr.push_back(raw_arg.data());
  }

  std::vector<voke::io::argument_t> args {};
  voke::io::fill(splitted_cstr.size(), splitted_cstr.data(), args);

  for (voke::io::argument_t &arg : args) {
    if (arg.prefix == "--tag") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.tag = arg.values.at(0);
    }

    if (arg.prefix == "--version") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.version = arg.values.at(0);
    }

    if (arg.prefix == "--libs") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.libs = arg.values;
    }

    if (arg.prefix == "--includes") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.includes = arg.values;
    }

    if (arg.prefix == "--compiler") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.compiler = arg.values.at(0);
    }

    if (arg.prefix == "--type") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.type = arg.values.at(0);
    }

    if (arg.prefix == "--arch") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.arch = arg.values.at(0);
    }
  }

  return voke::result::SUCCESS;
}

voke::flags_t voke::io::extract_library_info_from_line(
  std::string &line,
  voke::io::library_t &library
) {
  std::vector<std::string> splitted {voke::io::split(line, ' ')};
  splitted.insert(splitted.begin(), "meow ");

  std::vector<char*> splitted_cstr {};
  for (std::string &raw_arg : splitted) {
    splitted_cstr.push_back(raw_arg.data());
  }

  std::vector<voke::io::argument_t> args {};
  voke::io::fill(splitted_cstr.size(), splitted_cstr.data(), args);

  for (voke::io::argument_t &arg : args) {
    if (arg.prefix == "--type") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.type = arg.values.at(0);
    }

    if (arg.prefix == "--tag") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      voke::log() << library.tag;
    }

    if (arg.prefix == "--url") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.url = arg.values.at(0);
    }

    if (arg.prefix == "--git-clone-args") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.git_clone_args = arg.values;
    }

    if (arg.prefix == "--cmake-dir") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.cmake_dir = arg.values.at(0);
    }

    if (arg.prefix == "--generator") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.generator = arg.values.at(0);
    }

    if (arg.prefix == "--include-dirs") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.include_dirs = arg.values;
    }

    if (arg.prefix == "--binary-win-32-dirs") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.binary_win_32_dirs = arg.values;
    }

    if (arg.prefix == "--binary-win-64-dirs") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.binary_win_64_dirs = arg.values;
    }

    if (arg.prefix == "--binary-linux-32-dirs") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.binary_linux_32_dirs = arg.values;
    }

    if (arg.prefix == "--binary-linux-64-dirs") {
      if (arg.values.empty()) {
        return voke::result::ERROR_FAILED;
      }

      library.binary_linux_64_dirs = arg.values;
    }
  }

  return voke::result::SUCCESS;
}
