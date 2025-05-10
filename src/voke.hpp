#ifndef VOKE_HPP
#define VOKE_HPP

#include "argument/argument.hpp"
#include "resource/resource.hpp"

#include <string>
#include <vector>

namespace voke {
  extern struct app_t {
  public:
    std::string version {"0.1.0"};
    std::vector<std::string> raw_args {};
    std::vector<voke::argument_t> args {};
    std::vector<voke::compiler_t> installed_compilers {};
    std::vector<voke::compiler_t> compilers {};
    std::vector<voke::library_t> installed_libraries {};
    std::vector<voke::library_t> libraries {};
    std::unordored_map<std::string, std::vector<voke::target_t>> targets {}; 
    bool commands_state {};
  } app;
}

#endif
