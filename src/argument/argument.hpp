#ifndef VOKE_ARGUMENT_HPP
#define VOKE_ARGUMENT_HPP

#include "io/memory.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace voke {
  constexpr static size_t any {666};
  constexpr static size_t empty {0};
  constexpr static size_t not_empty {720};

  constexpr static bool must {true};
  constexpr static bool should {false};


  struct assembly_t {
  public:
    std::vector<std::string> prefixes {};
    size_t size {};
    bool must {};
    bool was_found {};
  };

  struct argument_compiler_info_t {
  public:
    std::string_view tag {};
    std::vector<std::string> lines {};
    std::vector<assembly_t> expect {};
  };

  struct argument_parser_info_t {
  public:
    std::vector<std::string> args {};
    bool serialize_quote {};
  };

  struct argument_t {
  public:
    std::string prefix {};
    std::vector<std::string> values {};
    std::string raw {};
    size_t line {};
  };
}

namespace voke::argument {
  voke::flags_t parse(
    voke::argument_parser_info_t &parser_info,
    std::vector<voke::argument_t> &argument_list
  );

  voke::flags_t compile(
    voke::argument_compiler_info_t &compile_info,
    std::vector<voke::argument_t> &argument_list
  );

  std::vector<voke::argument_t> find(
    const std::vector<std::string> &prefixes
  );

  voke::flags_t only(
    const std::vector<std::string> &prefixes
  );

  voke::flags_t any(
    const std::vector<std::string> &prefixes
  );

  voke::flags_t throw_unknown_command_or_arguments();
}

#endif
