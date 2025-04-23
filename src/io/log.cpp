#include "log.hpp"

std::ostringstream voke::log::buffer {};
bool voke::log::buffered {};
bool voke::log::tracked {};
int32_t voke::log::status {};