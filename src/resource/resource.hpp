#ifndef VOKE_RESOURCE_HPP
#define VOKE_RESOURCE_HPP

#include "argument/argument.hpp"
#include "io/log.hpp"

#include <vector>
#include <unordered_map>
#include <string>

namespace voke {
  typedef voke::argument value_t;
  typedef std::unordered_map<std::string, voke::value_t> compiler_t;
  typedef std::unordered_map<std::string, voke::value_t> library_t;
  typedef std::unordered_map<std::string, voke::value_t> target_t;
  typedef std::unordered_map<std::string, voke::value_t> operation_t;

  struct pack_info_t {
  public:
    std::string tag {};
    std::string type {};
    std::vector<voke::argument_t> compiled_arguments {};
  };
}

namespace voke::resource {
  template<typename t>
  voke::flags_t pack(
    voke::pack_info_t &pack_info,
    std::vector<t> &resources
  ) {
    t &resource {resources.emplace_back()};
    std::string prefix {};
    size_t underscores_pos {};
    std::unordered_map<std::string, size_t> repeated_fields {};
    for (voke::argument_t &argument : pack_info.compiled_arguments) {
      if (argument.prefix.empty()) {
        voke::log()
          << "fatal: empty argument prefix, could not pack '"
          << pack_info.tag << '\'';
        return voke::result::ERROR_FAILED;
      }

      prefix = argument.prefix;
      if ((underscores_pos = prefix.find("--")) != std::string::npos) {
        prefix.erase(prefix.begin(), prefix.begin() + underscores_pos);
      }

      if (resource.count(prefix)) {
        if (repeated_fields.count(prefix)) {
          prefix += std::to_string(++repeated_fields[prefix]);
        } else {
          prefix += std::to_string((repeated_fields[prefix] = 2));
        }
      } else {
        resource[prefix] = argument;
      }
    }

    return voke::result::SUCCESS;
  }

  template<typename t>
  voke::flags_t packs(
    voke::pack_info_t &pack_info,
    std::vector<t> &resources
  ) {
    size_t line {1};
    size_t size {pack_info.compiled_arguments.size()};
    size_t underscores_pos {};
    std::string prefix {};
    std::unordered_map<std::string, size_t> repeated_fields {};

    t resource { .type = pack_info.type };
    for (size_t it {}; it < size; it++) {
      voke::argument_t &argument {
        pack_info.compiled_arguments.at(it)
      };

      if (argument.prefix.empty()) {
        voke::log() << "fatal: empty argument prefix, could not pack";
        return voke::result::ERROR_FAILED;
      }
  
      if (argument.line != line) {
        resources.push_back(resource);
        resource = { .type = pack_info.type };
        line = argument.line;
      }
  
      prefix = argument.prefix;
      if ((underscores_pos = prefix.find("--")) != std::string::npos) {
        prefix.erase(prefix.begin(), prefix.begin() + underscores_pos);
      }

      if (resource.count(prefix)) {
        if (repeated_fields.count(prefix)) {
          prefix += std::to_string(++repeated_fields[prefix]);
        } else {
          prefix += std::to_string((repeated_fields[prefix] = 2));
        }
      } else {
        resource[prefix] = argument;
      }

      if (it == size - 1) {
        resources.push_back(resource);
      }
    }

    return voke::result::SUCCESS;
  }

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
