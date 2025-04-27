#ifndef VOKE_HPP
#define VOKE_HPP

#include "io/args.hpp"
#include "io/resource.hpp"

#include <string>
#include <vector>

namespace voke {
  enum verbose_level {
    LEVEL_ONE,
    LEVEL_TWO
  };

  extern struct app_t {
  public:
    voke::verbose_level verbose_level {voke::verbose_level::LEVEL_ONE};
    std::string version {"0.1.0"};
    std::vector<voke::io::argument_t> args {};
    std::vector<voke::io::compiler_t> installed_compilers {};
    std::vector<voke::io::compiler_t> compilers {};
    std::vector<voke::io::library_t> installed_libraries {};
    std::vector<voke::io::library_t> libraries {};
  } app;
}

#endif
