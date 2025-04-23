#include "git.hpp"
#include "log.hpp"

voke::flags_t voke::io::git_clone(std::string_view url) {
  voke::log() << std::system("git --version");

  return voke::result::SUCCESS;
}
