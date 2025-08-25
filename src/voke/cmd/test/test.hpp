#ifndef VOKE_CMD_TEST_HPP
#define VOKE_CMD_TEST_HPP

#include "voke/io/memory.hpp"
#include <vector>
#include <string>

namespace voke::cmd::test {
  voke::flags_t assert();
  voke::flags_t run();
}

#endif
