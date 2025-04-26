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

  return voke::result::ERROR_TIMEOUT;
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

  return voke::result::ERROR_TIMEOUT;
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
  std::vector<voke::io::argument_t> &args,
  bool should_serialize_quote
) {
  size_t size {static_cast<size_t>(args_size)};
  std::vector<std::string> unserialized_args {};
  std::string serialized_quote {};

  bool found_quote {};
  bool is_quoted {};

  for (size_t it {}; it < size; it++) {
    std::string arg {pp_args[it]};
    if (should_serialize_quote) {
      found_quote = (arg.front() == '\'' || arg.front() == '"');
      if (!is_quoted && found_quote) {
        is_quoted = true;
        arg.erase(arg.begin());
      }

      if (is_quoted) {
        serialized_quote += ' ' * !found_quote;
        serialized_quote += arg;
      }

      found_quote = (arg.back() == '\'' || arg.back() == '"');
      if (is_quoted && found_quote) {
        arg.erase(arg.end());
        is_quoted = false;
        unserialized_args.push_back(arg);
      } else if (!is_quoted) {
        unserialized_args.push_back(arg);
      }
    } else {
      unserialized_args.push_back(arg);
    }
  }

  bool is_at_end {};
  bool is_an_new_arg {};

  voke::io::argument_t serialized_arg {};  
  for (size_t it {1}; it < size; it++) {
    std::string arg {unserialized_args.at(it)};

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
