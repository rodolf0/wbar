cmake_minimum_required(VERSION 2.6)
project(Wbar)
set(Wbar_VERSION_MAJOR 3)
set(Wbar_VERSION_MINOR 0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(
  -std=c++11
  -g -O3
  -Wall
  -Wextra -Werror
  #-Wcast-qual # EINA commits some crimes here
  -Wshadow
  -Wpointer-arith
)

find_package(Evas REQUIRED)
find_package(X11 REQUIRED)

include_directories(
  ${EVAS_INCLUDE_DIRS}
  ${X11_INCLUDE_DIR}
  ${X11_Xrender_INCLUDE_PATH}
)

add_definitions(-include cstddef)

add_executable(Wbar
  Wbar.cc
  OptionParser.cc
  ConfigReader.cc
  CanvasEngine.cc
  LayoutStrategy.cc
  Geometry.cc
  Xwindow.cc
)

target_link_libraries(Wbar
  ${EVAS_LIBRARIES}
  ${X11_LIBRARIES}
  ${X11_Xrender_LIB}
)
