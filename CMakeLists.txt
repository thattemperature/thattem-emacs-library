set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 4.0)

project(Thattem_Emacs_Library)

add_library(thattem_emacs_library SHARED
  src/thattem_emacs_library.c
  src/gtop_data.c)


find_package(PkgConfig REQUIRED)

pkg_check_modules(GTOP REQUIRED
  IMPORTED_TARGET libgtop-2.0)
find_path(EMACS_INCLUDE_DIR emacs-module.h REQUIRED)


target_include_directories(thattem_emacs_library
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${EMACS_INCLUDE_DIR})
target_link_libraries(thattem_emacs_library
  PRIVATE
  PkgConfig::GTOP)


install(TARGETS thattem_emacs_library
  LIBRARY DESTINATION lib)
