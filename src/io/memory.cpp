#include "memory.hpp"
#include <sstream>
#include <iostream>

std::vector<std::string> voke::io::split(
  std::string &str,
  std::string delimiter
) {
  size_t pos {};
  size_t next_pos {};
  std::vector<std::string> splitted {};

  while ((pos = str.find(delimiter, next_pos)) != std::string::npos) {
    splitted.emplace_back() = str.substr(next_pos, pos - next_pos);
    next_pos = pos + delimiter.size();
  }

  splitted.emplace_back() = str.substr(next_pos);
  return splitted;
}
