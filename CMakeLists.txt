cmake_minimum_required(VERSION 3.20)
project(sdbg)

include_directories(vendor/linenoise include)

add_executable(sdbg src/simpldbg.cpp vendor/linenoise/linenoise.c)

set_target_properties(sdbg PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

set_source_files_properties(src/simpldbg.cpp PROPERTIES
  COMPILE_FLAGS "-std=c++17"
)

add_executable(hello examples/hello.cpp)
# set_target_properties(hello
#                       PROPERTIES COMPILE_FLAGS "-g -O0")
