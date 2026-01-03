cmake_minimum_required (VERSION 3.28)

option (ENABLE_TESTS "Enable tests" OFF)

# https://stackoverflow.com/questions/66680147/how-to-change-c-version-being-used-by-vs-code
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

project (bsgalone LANGUAGES CXX)

# https://cmake.org/cmake/help/latest/variable/CMAKE_LIBRARY_OUTPUT_DIRECTORY.html
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
# https://cmake.org/cmake/help/latest/variable/CMAKE_RUNTIME_OUTPUT_DIRECTORY.html
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

# https://cmake.org/cmake/help/v3.0/variable/CMAKE_VERBOSE_MAKEFILE.html
set (CMAKE_VERBOSE_MAKEFILE OFF)

# https://serverfault.com/questions/279068/cant-find-so-in-the-same-directory-as-the-executable
# https://stackoverflow.com/questions/57915564/cmake-how-to-set-rpath-to-origin-with-cmake
# https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_RPATH_USE_ORIGIN.html#variable:CMAKE_BUILD_RPATH_USE_ORIGIN
set (CMAKE_BUILD_RPATH_USE_ORIGIN true)

# https://www.reddit.com/r/cmake/comments/183gqzl/nice_way_to_apply_compilelink_options_to_all_of/
# https://github.com/saxbophone/CPP20-Cross-Platform-Template/blob/6991098e7f243ae9c6477c601953dd132f499e53/CMakeLists.txt#L77-L90
add_library (bsgalone-interface-library INTERFACE)
# https://stackoverflow.com/questions/5096881/does-set-target-properties-in-cmake-override-cmake-cxx-flags
target_compile_options (bsgalone-interface-library INTERFACE
	-Wall -Wextra -Werror
	)

add_library (bsgalone-interface-test-library INTERFACE)
# https://gcc.gnu.org/onlinedocs/gcc/Invoking-Gcov.html
target_compile_options (bsgalone-interface-test-library INTERFACE
	--coverage
	-fprofile-update=atomic
	-O0
	)
# https://gcovr.com/en/stable/cookbook.html#out-of-source-builds-with-cmake
target_link_options (bsgalone-interface-test-library INTERFACE
	--coverage
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/src
	)

# https://stackoverflow.com/questions/50468620/what-does-enable-testing-do-in-cmake
if (${ENABLE_TESTS})
	add_subdirectory (
		${CMAKE_CURRENT_SOURCE_DIR}/tests
		)
endif ()
