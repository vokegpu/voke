#ifndef VOKE_IO_VOKEFILE_HPP
#define VOKE_IO_VOKEFILE_HPP

#include "args.hpp"
#include "memory.h"

namespace voke {
  constexpr static size_t any {666};
  constexpr static size_t not_empty {720};

  struct assembly_t {
  public:
    prefix_list_t prefixes {};
    size_t size {};
    bool must {};
  };

  struct vokefile_parser_info {
  public:
    std::string_view tag {};
    std::string_view path {};
    std::vector<assembly_t> expect {};
  };
}

namespace voke::io {
  voke::flags_t vokefile_parser_to_args(
    voke::vokefile_parser_info &parser_info,
    voke::argument_list_t &argument_list
  );
}

#endif
