#ifndef VOKE_ARGUMENT_HPP
#define VOKE_ARGUMENT_HPP

#include "io/memory.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace voke {
  enum behavior : uint8_t {
    OPTIONAL = 2 << 1,
    MANDATORY = 2 << 2
  };

  struct assembly_t {
  public:
    std::vector<std::string> prefixes {};
    size_t size {};
    voke::flags_t behavior {};
    size_t was_found {};
    bool was_compiled {};
  };

  struct argument_compiler_info_t {
  public:
    std::string_view tag {};
    std::vector<std::string> lines {};
    bool match_first {};
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
  public:
    operator std::string() {
      if (this->values.empty()) {
        return "";
      }

      return this->values.at(0);
    }

    operator = (const std::string &value) {
      if (this->values.empty()) {
        this->values.emplace_back();
      }

      this->values.at(0) = value;
    }

    operator = (const char *p_value) {
      if (this->values.empty()) {
        this->values.emplace_back();
      }

      this->values.at(0) = std::string(p_value);
    }
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
