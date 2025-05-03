#include "compiler.hpp"

voke::flags_t voke::resource::serialize_compiler_from_argument(
  voke::argument_t &argument,
  voke::compiler_t &compiler
) {
  if (argument.prefix == "--tag") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    compiler.tag = argument.values.at(0);
  } else if (argument.prefix == "--binary-dir") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    compiler.binary_dir = argument.values.at(0);
  } else if (argument.prefix == "--lib-dir") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    compiler.lib_dir = argument.values.at(0);
  } else if (argument.prefix == "--include-dir") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    compiler.include_dir = argument.values.at(0);
  } else if (argument.prefix == "--c") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    compiler.c_compiler = argument.values.at(0);
  } else if (argument.prefix == "--cpp") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    compiler.cpp_compiler = argument.values.at(0);
  } else if (argument.prefix == "--version") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    compiler.version = argument.values.at(0);
  }

  return voke::result::SUCCESS;
}

voke::flags_t voke::resource::serialize_compiler_from_argument_list(
  std::vector<voke::argument_t> &argument_list, 
  voke::compiler_t &compiler
) {
  voke::flags_t result {};
  for (voke::argument_t &argument : argument_list) {
    result = voke::resource::serialize_compiler_from_argument(argument, compiler);
    if (result != voke::result::SUCCESS) {
      return result;
    }
  }

  return voke::result::SUCCESS;
}
