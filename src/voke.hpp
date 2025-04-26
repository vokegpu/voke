#ifndef VOKE_HPP
#define VOKE_HPP

#include "io/args.hpp"
#include "io/resource.hpp"

#include <string>
#include <vector>

namespace voke {
  extern struct app_t {
  public:
    std::string version {"0.1.0"};
    std::vector<voke::io::argument_t> args {};
    std::vector<voke::io::compiler_t> compilers {};
    std::vector<voke::io::library_t> libraries {};
  } app;
}

#endif
