#ifndef VOKE_IO_VOKEFILE_HPP
#define VOKE_IO_VOKEFILE_HPP

#include "memory.hpp"

#include <vector>
#include <string_view>

namespace voke::io {
  voke::flags_t vokefile_read_lines(
    std::string_view path,
    std::vector<std::string> &lines
  );

  voke::flags_t vokefile_query_files_from_dir(
    std::string_view dir_path,
    std::vector<std::string> &files
  );
}

#endif
