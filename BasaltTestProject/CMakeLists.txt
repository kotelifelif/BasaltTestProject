# CMakeList.txt : CMake project for BasaltTestProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("BasaltTestProject")

# Add source to this project's executable.
add_subdirectory(cpr)
add_subdirectory(json)
add_executable (BasaltTestProject "main.cpp" "Request.h")
target_link_libraries(BasaltTestProject PUBLIC pthread)
target_link_libraries(BasaltTestProject PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(BasaltTestProject PRIVATE cpr::cpr)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BasaltTestProject PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
