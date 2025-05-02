#include "vokefile.hpp"

voke::flags_t voke::io::vokefile_get_line_list(
  std::string path,
  voke::vokefile_line_list_t &line_list
) {
  voke::flags_t result {voke::result::ERROR_FAILED};
  std::ifstream file(parser_info.path);

  if (file.is_open()) {
    result = voke::result::SUCCESS;
    while (getline(file, line)) {
      if (line.empty()) {
        continue;
      }

      line_list.push_back(line);
    }

    file.close();
  }

  return line_list;
}

voke::flags_t voke::io::parser(
  voke::vokefile_parser_info_t &parser_info,
  voke::argument_list_t &argument_list
) {
  voke::flags_t result {voke::result::SUCCESS};
  if (!parser_info.line_list.empty()) {
    size_t argument_list_size {}
    size_t line_count {1};
    
    std::vector<std::string> splitted {};
    voke::argument_list_t filled_args {};

    bool must {};
    bool contains {};

    for (std::string &line : parser_info.line_list) {
      if (line.empty()) {
        continue;
      }

      splitted = voke::io::split(line, ' ');
      voke::io::fill(
        splitted,
        filled_args,
        true
      );

      argument_list_size = filled_args.size();
      for (size_t it {}; it < argument_list_size; it++) {
        argument_t &argument {filled_args.at(it)};
        argument.line = line_count;

        must_but_not_found = true;
        for (voke::assembly_t &assembly : parser_info.expect) {
          contains = false;
          for (std::string &prefix : assembly.prefixes) {
            if (
                (contains = prefix == argument.prefix)
              ) {
              break;
            }
          }

          if (!contains) {
            continue;
          }

          must_but_not_found = false;

          switch (assembly.size) {
          case voke::any:
            argument_list.push_back(argument);
            break;
          case voke::not_empty:
            if (!argument.values.empty()) {
              argument_list.push_back(argument);
              break;
            }

            voke::log()
              << "error: '" << parser_info.tag << "' line at:\n "
              << line_count << " | " << line;
              << " \t at column (" << (it+1) << "): '" << argument.prefix << "' invalid argument size, empty, expected arguments"; 
            break;
          default:
            if (argument.values.size() == assembly.size) {
              argument_list.push_back(argument);
              break;
            }

            voke::log()
              << "error: '" << parser_info.tag << "' line at:\n "
              << line_count << " | " << line;
              << " \t at column (" << (it+1) << "): '" << argument.prefix << "' invalid argument size '" << argument.values.size() << "' expected '" << assembly.size << '\''; 
            break;
          }
        }

        if (must_but_not_found) {
          voke::log() 
            << "error: '" << parser_info.tag << "' expected argument '" << argument.prefix << '\'';
        }
      }

      line_count++;
    }
  }

  return result;
}
