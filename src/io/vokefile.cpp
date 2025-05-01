#include "vokefile.hpp"

voke::flags_t voke::io::vokefile_parser_to_args(
  voke::vokefile_parser_info &parser_info,
  voke::argument_list_t &argument_list
) {
  voke::flags_t result {voke::result::SUCCESS};
  std::ifstream file(parser_info.path);

  if (file.is_open()) {
    std::string line {};
    std::vector<std::string> splitted {};
    
    size_t argument_list_size {}
    size_t line_count {1};
    
    voke::argument_list_t filled_args {};

    bool must {};
    bool contains {};

    while (getline(file, line)) {
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

    file.close();
  }

  return result;
}
