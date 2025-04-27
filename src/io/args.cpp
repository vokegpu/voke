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
  std::vector<voke::io::argument_t> &voke_args,
  voke::io::argument_t &arg
) {
  if (!arg.prefix.empty()) {
    voke_args.push_back(arg);
  }
}

void voke::io::extract(
  int32_t args_size,
  char **pp_args,
  std::vector<std::string> &extracted
) {
  extracted.reserve(args_size);
  for (int32_t it {}; it < args_size; it++) {
    extracted.push_back(pp_args[it]);
  }
}

void voke::io::fill(
  std::vector<std::string> &in_args,
  std::vector<voke::io::argument_t> &voke_args,
  bool should_serialize_quote
) {
  size_t size {in_args.size()};
  std::vector<std::string> unserialized_args {};

  std::string serialized_quote {};
  std::string quoted {};
  bool found_quote {};

  for (std::string arg : in_args) {
    if (arg.empty()) {
      continue;
    }

    if (should_serialize_quote) {
      found_quote = (arg.front() == '\'' || arg.front() == '"');
      if (quoted.empty() && found_quote) {
        quoted.clear();
        quoted += arg.front();
        arg.erase(arg.begin());
      }

      if (!quoted.empty()) {
        serialized_quote += ' ' * !found_quote;
        serialized_quote += arg;
      }

      found_quote = !quoted.empty() && (arg.back() == quoted.at(0));
      if (!quoted.empty() && found_quote) {
        quoted.clear();
        serialized_quote.pop_back();
        unserialized_args.push_back(serialized_quote);
      } else if (quoted.empty()) {
        unserialized_args.push_back(arg);
      }
    } else {
      unserialized_args.push_back(arg);
    }
  }

  size = unserialized_args.size();

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
        voke_args,
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
        voke_args,
        serialized_arg
      );
    }
  }
}
