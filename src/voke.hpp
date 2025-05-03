#ifndef VOKE_HPP
#define VOKE_HPP

#include "argument/argument.hpp"
#include "resource/compiler.hpp"
#include "resource/library.hpp"

#include <string>
#include <vector>

namespace voke {
  enum verbose_level {
    LEVEL_ONE,
    LEVEL_TWO
  };

  extern struct app_t {
  public:
    std::string version {"0.1.0"};
    std::vector<std::string> raw_args {};
    std::vector<voke::argument_t> args {};
    std::vector<voke::compiler_t> installed_compilers {};
    std::vector<voke::compiler_t> compilers {};
    std::vector<voke::library_t> installed_libraries {};
    std::vector<voke::library_t> libraries {};
    bool commands_state {};
  } app;
}

#endif
