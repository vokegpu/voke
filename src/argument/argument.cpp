#include "argument.hpp"
#include "voke.hpp"
#include "io/log.hpp"

voke::flags_t voke::argument::parse(
  voke::argument_parser_info_t &parser_info,
  std::vector<voke::argument_t> &argument_list
) {
  voke::flags_t result {voke::result::SUCCESS};
  std::vector<std::string> unserialized_args {};

  std::string serialized_quote {};
  std::string quoted {};
  bool found_quote {};

  for (std::string arg : parser_info.args) {
    if (arg.empty()) {
      continue;
    }

    if (parser_info.serialize_quote) {
      found_quote = (arg.front() == '\'' || arg.front() == '"');
      if (quoted.empty() && found_quote) {
        serialized_quote.clear();
        quoted = arg.front();
      }

      if (!quoted.empty()) {
        if (!found_quote) serialized_quote += ' ';
        serialized_quote += arg;
      }

      found_quote = !quoted.empty() && (arg.back() == quoted.at(0));
      if (!quoted.empty() && found_quote) {
        quoted.clear();
        unserialized_args.push_back(serialized_quote);
      } else if (quoted.empty()) {
        unserialized_args.push_back(arg);
      }
    } else {
      unserialized_args.push_back(arg);
    }
  }

  size_t size {unserialized_args.size()};

  bool is_at_end {};
  bool is_an_new_arg {};

  voke::argument_t serialized_arg {};  
  for (size_t it {1}; it < size; it++) {
    std::string arg {unserialized_args.at(it)};

    if (arg.empty()) {
      continue;
    }

    is_an_new_arg = arg.at(0) == '-' && arg.find(" ") == std::string::npos;
    is_at_end = it == size - 1;

    if (
      is_an_new_arg
      &&
      !serialized_arg.prefix.empty()
    ) {
      argument_list.push_back(serialized_arg);
    }

    if (is_an_new_arg) {
      serialized_arg = {
        .prefix=arg
      };

      serialized_arg.raw += serialized_arg.prefix;
    }

    if (arg.at(0) != '-' || (arg.at(0) == '-' && arg.find(" ") != std::string::npos)) {
      char c {};
      if ((c = arg.at(0)) == '\'' || c == '"') {
        arg.erase(arg.begin());
      }

      if (arg.at(arg.size() - !arg.empty()) == c) {
        arg.pop_back();
      }

      serialized_arg.values.push_back(arg);
      serialized_arg.raw += ' ';
      serialized_arg.raw += arg;
    }

    if (is_at_end && !serialized_arg.prefix.empty()) {
      argument_list.push_back(serialized_arg);
    }
  }

  return result;
}

voke::flags_t voke::argument::compile(
  voke::argument_compiler_info_t &compile_info,
  std::vector<voke::argument_t> &argument_list
) {
  voke::flags_t result {voke::result::SUCCESS};
  if (!compile_info.lines.empty()) {
    size_t size {};
    size_t line_count {1};

    std::vector<voke::argument_t> parsed_args {};
    voke::argument_parser_info_t parser_info {.serialize_quote = true};

    bool must {};
    size_t contains {};

    for (std::string &line : compile_info.lines) {
      if (line.empty()) {
        continue;
      }

      parser_info.args = voke::io::split(line, " ");
      voke::argument::parse(parser_info, parsed_args);

      size = parsed_args.size();
      for (size_t it {}; it < size; it++) {
        argument_t &argument {parsed_args.at(it)};
        argument.line = line_count;

        for (voke::assembly_t &assembly : compile_info.expect) {
          contains = 0;
          for (std::string &prefix : assembly.prefixes) {
            contains += prefix == argument.prefix;
          }

          if (contains != 1) {
            continue;
          }

          assembly.was_found = true;

          switch (assembly.size) {
          case voke::any:
            argument_list.push_back(argument);
            break;
          case voke::not_empty:
            if (!argument.values.empty()) {
              argument_list.push_back(argument);
              break;
            }

            result = voke::result::ERROR_FAILED;
            voke::log()
              << "error: '" << compile_info.tag << "' line at:\n "
              << line_count << " | " << line
              << " \t at column (" << (it+1) << "): '" << argument.prefix << "' invalid argument size, empty, expected arguments"; 
            break;
          default:
            if (argument.values.size() == assembly.size) {
              argument_list.push_back(argument);
              break;
            }

            result = voke::result::ERROR_FAILED;
            voke::log()
              << "error: '" << compile_info.tag << "' line at:\n "
              << line_count << " | " << line
              << " \t at column (" << (it+1) << "): '" << argument.prefix << "' invalid argument size '" << argument.values.size() << "' expected '" << assembly.size << '\''; 
            break;
          }
        }
      }

      line_count++;
    }

    std::string prefixes {};
    for (voke::assembly_t &assembly : compile_info.expect) {
      if (assembly.must && assembly.was_found) {
        continue;
      }

      prefixes.clear();
      size = assembly.prefixes.size();

      for (size_t it {}; it < size; it++) {
        prefixes += assembly.prefixes.at(it);

        if (it < size - 1) {
          prefixes += " | ";
        }
      }

      voke::log() 
        << "error: '" << compile_info.tag << "' expected argument '" << prefixes << '\'';
      result = voke::result::ERROR_FAILED;
    }
  }

  return result; 
}

voke::flags_t voke::argument::any(
  const std::vector<std::string> &prefixes
) {
  for (voke::argument_t &arg : voke::app.args) {
    for (const std::string &prefix : prefixes) {
      if (prefix == arg.prefix) {
        return voke::result::SUCCESS;
      }
    }
  }

  return voke::result::ERROR_TIMEOUT;
}

voke::flags_t voke::argument::only(
  const std::vector<std::string> &prefixes
) {
  bool contains_any {};
  for (voke::argument_t &arg : voke::app.args) {
    for (const std::string &prefix : prefixes) {
      if (
          (contains_any = prefix == arg.prefix)
      ) {
        break;
      }
    }

    if (!contains_any) {
      return voke::result::ERROR_FAILED;
    }
  }

  return voke::result::SUCCESS;
}

std::vector<voke::argument_t> voke::argument::find(
  const std::vector<std::string> &prefixes
) {
  std::vector<voke::argument_t> found_args {};
  for (voke::argument_t &arg : voke::app.args) {
    for (const std::string &prefix : prefixes) {
      if (arg.prefix == prefix) {
        found_args.push_back(arg);
        break;
      }
    }
  }

  return found_args;
}

voke::flags_t voke::argument::throw_unknown_command_or_arguments() {
  voke::log() << "error: unknown command or arguments use --help or -h";
  return voke::result::ERROR_UNKNOWN;
}
