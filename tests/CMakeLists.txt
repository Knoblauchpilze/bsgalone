
# http://google.github.io/googletest/quickstart-cmake.html
# https://cmake.org/cmake/help/latest/module/FetchContent.html
include (FetchContent)
FetchContent_Declare (
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        v1.15.2
)

FetchContent_MakeAvailable (googletest)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/unit
	)
