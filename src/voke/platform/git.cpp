#include "git.hpp"
#include "voke/io/log.hpp"
#include "voke/io/shell.hpp"
#include "voke/platform/os.hpp"

#include <iostream>
#include <string>

voke::flags_t voke::platform::sync_git_repository(
  voke::platform_git_sync_repository_info &sync_git_repository_info
) {
  voke::shell() << "cd " << sync_git_repository_info.path;
  if (voke::shell::result != 0) {
    voke::shell()
      << "git clone "
      << sync_git_repository_info.url
      << ' '
      << sync_git_repository_info.clone_args
      << ' '
      << sync_git_repository_info.path;
  }

  if (voke::shell::result) {
    voke::log::error = sync_git_repository_info.url;
    return voke::result::ERROR_FAILED;
  }

  if (sync_git_repository_info.enable_safe_directory) {
    voke::shell()
      << "git config --global --add safe.directory "
      << sync_git_repository_info.path;
  }

  voke::shell()
    << "cd "
    << sync_git_repository_info.path
    << " && git pull";

  return (
    voke::shell::result
    ?
    voke::result::ERROR_FAILED : voke::result::SUCCESS
  );
}
