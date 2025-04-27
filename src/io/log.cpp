#include "log.hpp"

std::ostringstream voke::log::buffer {};
bool voke::log::buffered {};
bool voke::log::tracked {};
std::string voke::log::error {};
int32_t voke::log::status {};
