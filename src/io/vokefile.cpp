#include "vokefile.hpp"

#include <fstream>

voke::flags_t voke::io::vokefile_read_lines(
  std::string path,
  std::vector<std::string> &lines
) {
  voke::flags_t result {voke::result::ERROR_FAILED};
  std::ifstream file(path);

  if (file.is_open()) {
    result = voke::result::SUCCESS;
    std::string line {};
    while (getline(file, line)) {
      if (line.empty()) {
        continue;
      }

      lines.push_back(line);
    }

    file.close();
  }

  return result;
}
