#include "vokefile.hpp"

#include <fstream>

voke::flags_t voke::io::vokefile_read_lines(
  std::string_view path,
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

voke::flags_t voke::io::vokefile_query_files_from_dir(
  std::string_view dir_path,
  std::vector<std::string> files
) {
  std::string path {};
  for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(library.voke_path)) {
    path = entry.path();
    if (path.rfind(".voke") == std::string::npos) {
      continue;
    }

    voke::log() << "detail: found voke-file at '" << path << '"'; 
  }

  return voke::result::SUCCESS;
}
