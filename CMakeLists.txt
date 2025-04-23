cmake_minimum_required(VERSION 3.15...3.31)

include(cmake/properties.cmake)

project(
  voke
  VERSION "${VOKE_VERSION}"
  DESCRIPTION "A modern lightweight library-manager cross-platform for C++/C"
  LANGUAGES CXX
)


include(cmake/specification.cmake)

message(STATUS "Voke building on ${PLATFORM}...")

file(
  GLOB_RECURSE
  SRC_FILES
  "src/*.cpp"
)

add_executable(voke ${SRC_FILES})

target_include_directories(
  voke PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:src>
)

set_target_properties(
  voke PROPERTIES
  CXX_STANDARD 17
)