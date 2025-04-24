#include "args.hpp"
#include "memory.hpp"
#include "voke.hpp"
#include "log.hpp"

#include <regex>

voke::flags_t voke::io::args_contains_any(
  std::vector<std::string> prefixes
) {
  for (voke::io::argument_t &arg : voke::app.args) {
    for (std::string &prefix : prefixes) {
      if (prefix == arg.prefix) {
        return voke::result::SUCCESS;
      }
    }
  }

  return voke::result::TIMEOUT;
}

voke::flags_t voke::io::args_contains_any_non(
  std::vector<std::string> prefixes
) {
  bool contains_any {};
  for (voke::io::argument_t &arg : voke::app.args) {
    for (std::string &prefix : prefixes) {
      if (
          (contains_any = prefix == arg.prefix)
      ) {
        break;
      }
    }

    if (!contains_any) {
      return voke::result::SUCCESS;
    }
  }

  return voke::result::TIMEOUT;
}

std::vector<voke::io::argument_t> voke::io::args_find_all(
  std::vector<std::string> prefixes
) {
  std::vector<voke::io::argument_t> found_args {};
  for (voke::io::argument_t &arg : voke::app.args) {
    for (std::string &prefix : prefixes) {
      if (arg.prefix == prefix) {
        found_args.push_back(arg);
        break;
      }
    }
  }

  return found_args;
}

void voke::io::push_back_arg_if_necessary(
  std::vector<voke::io::argument_t> &args,
  voke::io::argument_t &arg
) {
  if (!arg.prefix.empty()) {
    args.push_back(arg);
  }
}

void voke::io::fill(
  int32_t args_size,
  char **pp_args,
  std::vector<voke::io::argument_t> &args
) {
  size_t size {static_cast<size_t>(args_size)};
  std::vector<std::string> unserialized_args {};

  for (size_t it {}; it < size; it++) {
    unserialized_args.push_back(pp_args[it]);
  }

  bool is_at_end {};
  bool is_an_new_arg {};

  voke::io::argument_t serialized_arg {};  
  for (size_t it {1}; it < size; it++) {
    std::string &arg {unserialized_args.at(it)};

    if (arg.empty()) {
      continue;
    }

    is_an_new_arg = arg.at(0) == '-';
    is_at_end = it == size - 1;

    if (
      is_an_new_arg
      &&
      !serialized_arg.prefix.empty()
    ) {
      voke::io::push_back_arg_if_necessary(
        args,
        serialized_arg
      );
    }

    if (is_an_new_arg) {
      serialized_arg = {
        .prefix=arg
      };

      serialized_arg.raw += serialized_arg.prefix;
    }

    if (arg.at(0) != '-') {
      serialized_arg.values.push_back(arg);
      serialized_arg.raw += ' ';
      serialized_arg.raw += arg;
    }

    if (is_at_end) {
      voke::io::push_back_arg_if_necessary(
        args,
        serialized_arg
      );
    }
  }
}
