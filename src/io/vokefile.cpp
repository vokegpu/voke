#include "vokefile.hpp"

voke::flags_t voke::io::voekfile_read_lines(
  std::string path,
  std::vector<std::string> &lines
) {
  voke::flags_t result {voke::result::ERROR_FAILED};
  std::ifstream file(parser_info.path);

  if (file.is_open()) {
    result = voke::result::SUCCESS;
    while (getline(file, line)) {
      if (line.empty()) {
        continue;
      }

      lines.push_back(line);
    }

    file.close();
  }

  return lines;
}
