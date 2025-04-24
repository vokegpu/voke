#include "memory.hpp"
#include <sstream>

std::vector<std::string> voke::io::split(std::string &str, char delimiter) {
  std::vector<std::string> splitted {};
  std::stringstream ss(str);
  std::string token {};

  while (std::getline(ss, token, delimiter)) {
    splitted.push_back(token);
  }

  return splitted;
}
