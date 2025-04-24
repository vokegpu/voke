#include "git.hpp"
#include "os.hpp"
#include "io/log.hpp"

#include <iostream>
#include <string>

void voke::platform::voke_system_git_sync() {
  std::string arg_builder {};

  arg_builder = {};
  arg_builder += "cd ";
  arg_builder += voke::platform::voke_system_path;

  if (std::system(arg_builder.c_str()) != 0) {
    arg_builder = {};
    arg_builder += "git clone ";
    arg_builder += voke::platform::vokegpu_voke_libraries_repository_url;
    arg_builder += ' ';
    arg_builder += voke::platform::voke_system_path;

    voke::log::status = std::system(arg_builder.c_str());

    arg_builder = {};
    arg_builder += "cd ";
    arg_builder += voke::platform::voke_system_path;

    voke::log::status = std::system(arg_builder.c_str());
  }

  voke::log::status = std::system("git pull");
}
