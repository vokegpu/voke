#include "library.hpp"

voke::flags_t voke::resource::serialize_library_from_argument(
  voke::argument_t &argument,
  voke::library_t &library
) {
  if (arg.prefix == "--tag") {
    if (arg.values.empty()) {
      voke::log::error = arg.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.tag = arg.values.at(0);
  } else if (arg.prefix == "--version") {
    if (arg.values.empty()) {
      voke::log::error = arg.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.version = arg.values.at(0);
  } else if (arg.prefix == "--libs") {
    if (arg.values.empty()) {
      voke::log::error = arg.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.libs = arg.values;
  } else if (arg.prefix == "--includes") {
    if (arg.values.empty()) {
      voke::log::error = arg.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.includes = arg.values;
  } else if (arg.prefix == "--compiler") {
    if (arg.values.empty()) {
      voke::log::error = arg.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.compiler = arg.values.at(0);
  } else if (arg.prefix == "--type") {
    if (arg.values.empty()) {
      voke::log::error = arg.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.type = arg.values.at(0);
  } else if (arg.prefix == "--arch") {
    if (arg.values.empty()) {
      voke::log::error = arg.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.arch = arg.values.at(0);
  } else if (argument.prefix == "--target") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.target = argument.values;
  } else if (argument.prefix == "--build-system") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.build_system = argument.values.at(0);
    voke::log() << library.build_system;
  } else if (argument.prefix == "--url") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.url = argument.values.at(0);
  } else if (argument.prefix == "--git-clone-args") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.git_clone_args = argument.values.at(0);
  } else if (argument.prefix == "--include-dirs") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.include_dirs = argument.values;
  } else if (argument.prefix == "--binary-win-32-dirs") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.binary_win_32_dirs = argument.values;
  } else if (argument.prefix == "--binary-win-64-dirs") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.binary_win_64_dirs = argument.values;
  } else if (argument.prefix == "--binary-linux-32-dirs") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.binary_linux_32_dirs = argument.values;
  } else if (argument.prefix == "--binary-linux-64-dirs") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.binary_linux_64_dirs = argument.values;
  } else if (argument.prefix == "--run") {
    if (argument.values.empty()) {
      voke::log::error = argument.prefix;
      return voke::result::ERROR_FAILED;
    }

    library.run = argument.values.at(0);
  }
}

voke::flags_t voke::resource::serialize_library_from_argument_list(
  std::vector<voke::argument_t> &argument_list,
  voke::library_t &library
) {
  voke::flags_t result {};
  for (voke::argument_t &argument : argument_list) {
    result = voke::resource::serialize_library_from_argument(argument, library);
    if (result != voke::result::SUCCESS) {
      return result;
    }
  }

  return voke::result::SUCCESS;
}
