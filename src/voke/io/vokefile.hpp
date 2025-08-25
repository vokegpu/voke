#ifndef VOKE_IO_VOKEFILE_HPP
#define VOKE_IO_VOKEFILE_HPP

#include "memory.hpp"

#include <vector>

namespace voke::io {
  voke::flags_t vokefile_read_lines(
    const std::string &path,
    std::vector<std::string> &lines
  );

  voke::flags_t vokefile_query_files_from_dir(
    const std::string &dir_path,
    std::vector<std::string> &files
  );
}

#endif
