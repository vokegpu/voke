#ifndef VOKE_RESOURCE_HPP
#define VOKE_RESOURCE_HPP

#include "argument/argument.hpp"
#include "io/log.hpp"

#include <vector>
#include <unordered_map>
#include <string>

namespace voke {
  typedef std::unordered_map<std::string, voke::argument_t> compiler_t;
  typedef std::unordered_map<std::string, voke::argument_t> library_t;
}

namespace voke::resource {
  template<typename t>
  size_t mandatory(
    const std::vector<std::vector<string>> &mandatory_fields,
    t &resource
  ) {
    voke::log() << "detail: checking for conflict(s) in '" << resource["tag"] << '\'';
    size_t error_counter {};
    bool contains {};
    for (const std::vector<std::string> &fields : mandatory_fields) {
      contains = false;
      for (const std::string &field : fields) {
        if ((contains = field.count(resource))) {
          break;
        }
      }

      if (contains) {
        continue;
      }

      voke::log() << "fatal: failed to find mandatory field '" << field << '\'';
      error_counter++;
    }

    return error_counter;
  }
}

#endif
