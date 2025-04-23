#ifndef VOKE_IO_GIT_HPP
#define VOKE_IO_GIT_HPP

#include "memory.hpp"

#include <string_view>

namespace voke::io {
  voke::flags_t git_clone(
    std::string_view url
  );
};

#endif