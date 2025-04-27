#include "git.hpp"
#include "io/log.hpp"
#include "platform/os.hpp"

#include <iostream>
#include <string>

voke::flags_t voke::platform::sync_git_repository(
  std::string url,
  std::string path_to_clone,
  std::string clone_args
) {
  voke::shell() << "cd " << path_to_clone;
  if (voke::shell::result != 0) {
    voke::shell() 
      << "git clone "
      << url
      << ' '
      << clone_args
      << ' '
      << path_to_clone;
  }

  if (voke::shell::result) {
    voke::log::error = url;
    return voke::result::ERROR_FAILED;
  }

  voke::shell() 
    << "cd "
    << path_to_clone
    << " && git pull";

  return (
    voke::shell::result
    ?
    voke::result::SUCCESS : voke::result::ERROR_FAILED
  );
}
