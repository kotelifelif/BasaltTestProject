# CMakeList.txt : CMake project for request_library, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("request_library")

# Add source to this project's executable.
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
add_library(${PROJECT_NAME} SHARED
    request_library.cpp
    request_library.h
)

target_link_libraries(request_library PUBLIC pthread)
target_link_libraries(request_library PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(request_library PUBLIC cpr::cpr)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET request_library PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
