# CMakeList.txt : CMake project for request_library, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("request_library")

# Add source to this project's executable.
add_subdirectory(cpr)
add_subdirectory(json)
add_library(${PROJECT_NAME} SHARED
    request_library.cpp
    request_library.h
)

target_link_libraries(request_library PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(request_library PUBLIC cpr::cpr)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET request_library PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
