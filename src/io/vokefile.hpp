#ifndef VOKE_IO_VOKEFILE_HPP
#define VOKE_IO_VOKEFILE_HPP

#include "memory.h"

#include <vector>
#include <string>

namespace voke::io {
  voke::flags_t voekfile_read_lines(
    std::string path,
    std::vector<std::string> &lines
  );
}

#endif
