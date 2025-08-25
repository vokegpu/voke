#include "vokefile.hpp"
#include "voke/io/log.hpp"

#include <fstream>
#include <iostream>
#include <filesystem>

voke::flags_t voke::io::vokefile_read_lines(
  const std::string &path,
  std::vector<std::string> &lines
) {
  voke::flags_t result {voke::result::ERROR_FAILED};
  std::ifstream file(path.data());

  if (file.is_open()) {
    result = voke::result::SUCCESS;
    std::string line {};
    size_t pos {};
    while (getline(file, line)) {
      if (line.empty()) {
        continue;
      }

      if ((pos = line.rfind("\n")) != std::string::npos) {
        line.erase(line.begin() + pos);
      }

      if ((pos = line.rfind("\r")) != std::string::npos) {
        line.erase(line.begin() + pos);
      }

      lines.push_back(line);
    }

    file.close();
  }

  return result;
}

voke::flags_t voke::io::vokefile_query_files_from_dir(
  const std::string &dir_path,
  std::vector<std::string> &files
) {
  std::string path {};
  for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(dir_path)) {
    path = entry.path();
    if (path.rfind(".voke") == std::string::npos) {
      continue;
    }

    voke::log() << "detail: found voke-file at '" << path << '\'';
    files.push_back(path);
  }

  return voke::result::SUCCESS;
}
