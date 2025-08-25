#ifndef VKLANG_PARSER_HPP
#define VKLANG_PARSER_HPP

#include <vector>
#include "typing.hpp"
#include "vklang/io/log.hpp"

namespace vklang {
  template<vklang::parser_typing_type typing_type>
  struct parser_fragment_t {
  public:
    vklang::parser_typing_type type {typing_type};
  };

  struct parser_set_t {
  public:
    std::vector<vklang::parser_fragment_t<vklang::parser_typing_type::STR>>
      str_fragment {};
    std::vector<vklang::parser_fragment_t<vklang::parser_typing_type::I32>>
      i32_fragment {};
  };
}

namespace vklang::parser {
  constexpr std::regex rg_var_declaration {
    std::regex(": +([a-zA-Z0-9_.-]*) +")
  };

  constexpr std::regex rg_var_assign {
    std::regex(": +([a-zA-Z0-9_.-]*) +")
  };

  vklang::result peek(
    vklang::parser_set_t &set,
    std::string_view text
  );
}

#endif
