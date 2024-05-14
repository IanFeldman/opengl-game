cmake_minimum_required(VERSION 3.0)
project(OpenGL-Test)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
    pkg_check_modules(epoxy REQUIRED epoxy)
    pkg_check_modules(glfw3 REQUIRED glfw3)

include_directories(${CMAKE_INCLUDE_PATH})

add_executable(OpenGL-Test main.cpp)

target_link_libraries(OpenGL-Test ${CMAKE_LIBRARY_PATH} glfw GL epoxy wayland-client wayland-cursor wayland-egl)

