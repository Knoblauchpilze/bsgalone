cmake_minimum_required (VERSION 3.7)

# https://stackoverflow.com/questions/66680147/how-to-change-c-version-being-used-by-vs-code
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# https://stackoverflow.com/questions/5096881/does-set-target-properties-in-cmake-override-cmake-cxx-flags
add_compile_options (-Wall -Wextra -Werror)

project (bsgalone LANGUAGES CXX)

# https://cmake.org/cmake/help/latest/variable/CMAKE_LIBRARY_OUTPUT_DIRECTORY.html
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
# https://cmake.org/cmake/help/latest/variable/CMAKE_RUNTIME_OUTPUT_DIRECTORY.html
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

# https://cmake.org/cmake/help/v3.0/variable/CMAKE_VERBOSE_MAKEFILE.html
set (CMAKE_VERBOSE_MAKEFILE OFF)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/src
	)

enable_testing ()

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/tests
	)
