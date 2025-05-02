#ifndef VOKE_IO_VOKEFILE_HPP
#define VOKE_IO_VOKEFILE_HPP

#include "args.hpp"
#include "memory.h"

namespace voke {
  constexpr static size_t any {666};
  constexpr static size_t not_empty {720};

  typedef std::vector<std::string> vokefile_line_list_t;

  struct assembly_t {
  public:
    prefix_list_t prefixes {};
    size_t size {};
    bool must {};
  };

  struct vokefile_parser_info_t {
  public:
    std::string_view tag {};
    voke::vokefile_line_list_t line_list {};
    std::vector<assembly_t> expect {};
  };
}

namespace voke::io {
  voke::flags_t vokefile_get_line_list(
    std::string path,
    voke::vokefile_line_list_t &line_list
  );

  voke::flags_t parser(
    voke::vokefile_parser_info_t &parser_info,
    voke::argument_list_t &argument_list
  );
}

#endif
