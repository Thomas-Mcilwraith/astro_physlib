cmake_minimum_required(VERSION 3.20)
project(psf_physlib LANGUAGES C)

# Compiler settings ----------------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/c)

# Subdirectories -------------------------------------------------------------

add_subdirectory(src/libraries/c)
add_subdirectory(src/applications/c)
