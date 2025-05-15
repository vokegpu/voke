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
  };

  struct argument_compiler_info_t {
  public:
    std::string tag {};
    std::vector<std::string> lines {};
    bool match_first {};
    bool allow_repeated_arguments {};
    std::vector<assembly_t> expect {};
  };

  struct argument_parser_info_t {
  public:
    std::vector<std::string> args {};
  };

  struct argument_t {
  public:
    std::string prefix {};
    std::vector<std::string> values {};
    std::string raw {};
    size_t line {};
  public:
    argument_t() = default;

    argument_t(size_t line) {
      this->line = line;
    }

    argument_t(const std::string &first_value) {
      this->values.emplace_back() = first_value;
    }

    argument_t(const char *p_first_value) {
      this->values.emplace_back() = std::string(p_first_value);
    }

    operator std::string&() {
      return this->values.at(0); // @TODO: chek if explode
    }

    operator std::string() {
      if (this->values.empty()) {
        return "";
      }

      return this->values.at(0);
    }

    bool operator == (const voke::argument_t &l) {
      return this->line == l.line;
    }

    bool operator != (const voke::argument_t &l) {
      return this->line != l.line;
    }
  };
}

namespace voke::argument {
  extern voke::argument_t not_found;

  voke::flags_t parse(
    voke::argument_parser_info_t &parser_info,
    std::vector<voke::argument_t> &argument_list
  );

  voke::flags_t compile(
    voke::argument_compiler_info_t &compile_info,
    std::vector<voke::argument_t> &argument_list
  );

  voke::argument_t &find(
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
