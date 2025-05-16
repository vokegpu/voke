#ifndef VOKE_RESOURCE_HPP

#define VOKE_RESOURCE_HPP

#include "argument/argument.hpp"
#include "io/log.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>

namespace voke {
  typedef voke::argument_t value_t;
  typedef std::unordered_map<std::string, voke::value_t> compiler_t;
  typedef std::unordered_map<std::string, voke::value_t> library_t;
  typedef std::unordered_map<std::string, voke::value_t> target_t;
  typedef std::unordered_map<std::string, voke::value_t> operation_t;

  struct resource_pack_info_t {
  public:
    std::string tag {};
    std::string type {};
    std::vector<voke::argument_t> compiled_arguments {};
  };

  template<typename t>
  struct resource_query_info_t {
  public:
    std::function<bool(t &resource)> predicate {};
    std::vector<t> &resources;
  };
}

namespace voke::resource {
  template<typename t>
  voke::flags_t pack(
    voke::resource_pack_info_t &pack_info,
    t &resource
  ) {
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
        prefix.erase(prefix.begin() + underscores_pos, prefix.begin() + 2);
      }

      if (resource.count(prefix)) {
        if (repeated_fields.count(prefix)) {
          prefix += std::to_string(++repeated_fields[prefix]);
        } else {
          prefix += std::to_string((repeated_fields[prefix] = 2));
        }
      }

      resource[prefix] = argument;
    }

    return voke::result::SUCCESS;
  }

  template<typename t>
  voke::flags_t packs(
    voke::resource_pack_info_t &pack_info,
    std::vector<t> &resources
  ) {
    size_t line {1};
    size_t size {pack_info.compiled_arguments.size()};
    size_t underscores_pos {};
    std::string prefix {};
    std::unordered_map<std::string, size_t> repeated_fields {};

    t resource { {"type", pack_info.type} };
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
        resource["type"] = pack_info.type;
        line = argument.line;
        resource = {};
      }

      prefix = argument.prefix;
      if ((underscores_pos = prefix.find("--")) != std::string::npos) {
        prefix.erase(prefix.begin() + underscores_pos, prefix.begin() + 2);
      }

      if (resource.count(prefix)) {
        if (repeated_fields.count(prefix)) {
          prefix += std::to_string(++repeated_fields[prefix]);
        } else {
          prefix += std::to_string((repeated_fields[prefix] = 2));
        }
      }

      resource[prefix] = argument;

      if (it == size - 1) {
        resources.push_back(resource);
      }
    }

    return voke::result::SUCCESS;
  }

  template<typename t>
  voke::flags_t query(
    voke::resource_query_info_t<t> &query_info,
    t &resource
  ) {
    return (
      (
        std::find_if(
          query_info.resources.begin(),
          query_info.resources.end(),
          [&](t &item) {
            if (query_info.predicate(item)) {
              resource = item;
              return true;
            }

            return false;
          }
        ) != query_info.resources.end()
      ) ?
      voke::result::ERROR_TIMEOUT
      :
      voke::result::SUCCESS
    );
  }
}

#endif
