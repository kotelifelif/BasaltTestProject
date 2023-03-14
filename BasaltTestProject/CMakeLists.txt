# CMakeList.txt : CMake project for BasaltTestProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("BasaltTestProject")

# Add source to this project's executable.
add_subdirectory(request_library)
add_executable (BasaltTestProject "main.cpp")
target_link_libraries(BasaltTestProject request_library)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BasaltTestProject PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
install(TARGETS BasaltTestProject DESTINATION bin)