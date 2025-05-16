#include "argument.hpp"
#include "voke.hpp"
#include "io/log.hpp"

voke::argument_t voke::argument::not_found {voke::empty};

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
  }

  size_t size {unserialized_args.size()};

  bool is_at_end {};
  bool is_an_new_arg {};
  bool is_quoted {};

  voke::argument_t serialized_arg {};  
  for (size_t it {}; it < size; it++) {
    std::string arg {unserialized_args.at(it)};

    if (arg.empty()) {
      continue;
    }

    is_an_new_arg = arg.at(0) == '-' && !is_quoted;
    is_at_end = it == size - 1;

    if (
      is_an_new_arg
      &&
      !serialized_arg.prefix.empty()
    ) {
      argument_list.push_back(serialized_arg);
    }

    if (is_an_new_arg) {
      serialized_arg = {};
      serialized_arg.prefix = arg;

      serialized_arg.raw += serialized_arg.prefix;
    }

    if (arg.at(0) != '-' || is_quoted) {
      char c {};
      if ((c = arg.at(0)) == '\'' || c == '"') {
        arg.erase(arg.begin());
        is_quoted = true;
      }

      if (is_quoted && arg.at(arg.size() - !arg.empty()) == c) {
        is_quoted = false;
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
  voke::argument_compiler_info_t &compiler_info,
  std::vector<voke::argument_t> &argument_list
) {
  voke::flags_t result {voke::result::SUCCESS};

  size_t size {};
  size_t line_count {1};

  if (!compiler_info.lines.empty()) {
    std::vector<voke::argument_t> parsed_args {};
    voke::argument_parser_info_t parser_info {};

    bool found_prefix {};
    bool found {};
    bool first_matched {};
    size_t previous_line {};

    for (std::string &line : compiler_info.lines) {
      if (line.empty()) {
        continue;
      }

      parser_info.args = voke::io::split(line, " ");
      parsed_args.clear();

      voke::argument::parse(parser_info, parsed_args);
      size = parsed_args.size();

      for (size_t it {}; it < size; it++) {
        argument_t &argument {parsed_args.at(it)};
        argument.line = line_count;

        if (previous_line != line_count) {
          for (voke::assembly_t &assembly : compiler_info.expect) {
            assembly.was_found = 0;
          }

          previous_line = line_count;
        }

        found = false;
        for (voke::assembly_t &assembly : compiler_info.expect) {
          if (
            !compiler_info.allow_repeated_arguments
            &&
            assembly.was_found == 1
          ) {
            continue;
          }

          found_prefix = false;
          for (std::string &prefix : assembly.prefixes) {
            if (prefix == argument.prefix) {
              found_prefix = true;
              assembly.was_found++;
            }
          }

          if (!compiler_info.allow_repeated_arguments && compiler_info.match_first && assembly.was_found != 1 && it == 0 && !first_matched) {
            first_matched = true;
            return voke::result::ERROR_FAILED;
          }

          if (
            !compiler_info.allow_repeated_arguments
            &&
            assembly.was_found > 1
          ) {
            continue;
          }

          if (!found_prefix) {
            continue;
          }

          found = true;
          first_matched = true;

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
              << "error: '" << compiler_info.tag << "' at:\n "
              << line_count << " | " << line
              << "\n    at column (" << (it+1) << "): '" << argument.prefix << "' invalid argument size, empty, expected arguments"; 
            break;
          default:
            if (argument.values.size() == assembly.size) {
              argument_list.push_back(argument);
              break;
            }

            result = voke::result::ERROR_FAILED;
            voke::log()
              << "error: '" << compiler_info.tag << "' at:\n "
              << line_count << " | " << line
              << "\n    at column (" << (it+1) << "): '" << argument.prefix << "' invalid argument size '" << argument.values.size() << "' expected '" << assembly.size << '\'';
            break;
          }
        }

        if (!found) {
          std::string expected_raw {};
          for (voke::assembly_t &assembly : compiler_info.expect) {
            if (assembly.was_found) {
              continue;
            }

            size = assembly.prefixes.size();
            expected_raw += "     | ";
            for (size_t it {}; it < size; it++) {
              expected_raw += ' ';
              expected_raw += assembly.prefixes.at(it);
            }

            expected_raw += '\n';
          }

          if (expected_raw.empty()) {
            voke::log()
              << "error: '" << compiler_info.tag << "' at:\n "
              << line_count << " | " << line
              << "\n    at column (" << (it+1) << "): '" << argument.prefix << "' unknown argument";
          } else {
            expected_raw.pop_back(); // remove last '\n' char
            voke::log()
              << "error: '" << compiler_info.tag << "' at:\n "
              << line_count << " | " << line
              << "\n    at column (" << (it+1) << "): '" << argument.prefix << "' unknown argument, expected:\n"
              << expected_raw;
          }

          result = voke::result::ERROR_FAILED;
          break;
        }
      }

      line_count++;
    }
  } else {
    result = voke::result::ERROR_FAILED;
  }

  std::string prefixes {};
  bool first_match {true};
  bool is_mandatory {};

  for (voke::assembly_t &assembly : compiler_info.expect) {
    is_mandatory = voke::io::has(assembly.behavior, voke::behavior::MANDATORY);
    if (!is_mandatory || (is_mandatory && assembly.was_found)) {
      first_match = false;        
      continue;
    }

    result = voke::result::ERROR_FAILED;
    if (first_match && compiler_info.match_first) {
      break;
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
      << "error: '" << compiler_info.tag << "' expected argument '" << prefixes << '\'';
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

voke::argument_t &voke::argument::find(
  const std::vector<std::string> &prefixes
) {
  for (voke::argument_t &arg : voke::app.args) {
    for (const std::string &prefix : prefixes) {
      if (arg.prefix == prefix) {
        return arg;
      }
    }
  }

  return voke::argument::not_found;
}

voke::flags_t voke::argument::throw_unknown_command_or_arguments() {
  voke::log() << "error: unknown options use --help or -h";
  return voke::result::ERROR_UNKNOWN;
}
