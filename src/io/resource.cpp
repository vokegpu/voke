#include "resource.hpp"
#include "args.hpp"
#include "log.hpp"

#include <algorithm>

voke::flags_t voke::io::extract_compiler_info_from_line(
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
    if (arg.prefix == "--name") {
      if (arg.values.empty()) {
        return voke::result::FAILED;
      }

      compiler.voke_tag = arg.values.at(0);
    }

    if (arg.prefix == "--binary-dir") {
      if (arg.values.empty()) {
        return voke::result::FAILED;
      }

      compiler.binary_dir = arg.values.at(0);
    }
  }

  return voke::result::SUCCESS;
}
