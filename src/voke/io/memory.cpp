#include "memory.hpp"
#include "shell.hpp"

#include <sstream>
#include <iostream>
#include <regex>

std::vector<std::string> voke::io::split(
  std::string &str,
  std::string delimiter
) {
  size_t pos {};
  size_t next_pos {};
  size_t delimiter_size {delimiter.size()};

  std::vector<std::string> splitted {};

  while ((pos = str.find(delimiter, next_pos)) != std::string::npos) {
    splitted.emplace_back() = str.substr(next_pos, pos - next_pos);
    next_pos = pos + delimiter_size;
  }

  splitted.emplace_back() = str.substr(next_pos);
  return splitted;
}

void voke::io::cut_end_of_url(
  std::string &url,
  std::string &end
) {
  if (end.back() == '/') {
    end.pop_back();
  }

  size_t slash {url.rfind("/")};
  if (slash != std::string::npos) {
    end = url.substr(slash, url.size());
  }
}

void voke::io::replace(
  std::string &str_to_replace,
  const std::string &to_replace,
  const std::string &replace
) {
  str_to_replace = std::regex_replace(str_to_replace, std::regex(to_replace), replace);
}

voke::flags_t voke::io::copy(
  const std::string &src,
  const std::string &dir
) {
  if (src.empty() || dir.empty()) {
    return voke::result::ERROR_FAILED;
  }

  voke::shell() << "sudo -k";

  bool is_folder {
    src.back() == '\''
    ||
    src.back() == '\\'
    ||
    src.back() == '*'
  };

  #if defined(_WIN32) || defined(_WIN64)
    // @TODO: windows
  #elif defined(__linux__)
    if (is_folder) {
      voke::shell() << "sudo cp -r " << src << ' ' << dir;
    } else {
      voke::shell() << "sudo cp " << src << ' ' << dir;
    }
  #endif

  return (
  voke::shell::result == 0
    ?
    voke::result::SUCCESS
    :
    voke::result::ERROR_FAILED
  );
}
