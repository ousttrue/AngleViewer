CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(AngleViewer VERSION 0.1.0)

set(CMAKE_INSTALL_PREFIX $ENV{VCPKG_DIR}/installed/x64-windows)

add_definitions(-DNOMINMAX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/glTF-SDK/Build/CMake/Modules")
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(rapidjson/include)
add_subdirectory(glTF-SDK/GLTFSDK)

add_library(RapidJSON dumy.c)

SUBDIRS(scene gui gles3renderer src)

