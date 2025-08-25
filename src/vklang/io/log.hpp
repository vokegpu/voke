#ifndef VKLANG_IO_LOG_HPP
#define VKLANG_IO_LOG_HPP

#define vklang_log(content) std::cout << content << std::endl;

namespace vklang {
  enum class result : uint64_t {
    SUCCESS,
    FAILED
  };
}

#endif
