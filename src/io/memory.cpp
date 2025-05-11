#include "memory.hpp"
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
  std::string to_replace,
  std::string replace
) {
  str_to_replace = std::regex_replace(str_to_replace, std::regex(to_replace), replace);
}
