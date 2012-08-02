cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

project(Wbar)

# http://svn.enlightenment.org/svn/e/trunk/PROTO/eobj/cmake/Modules/
find_package(Evas REQUIRED)
find_package(Eina REQUIRED)
find_package(X11 REQUIRED)

include_directories(
  ${EVAS_INCLUDE_DIRS}
  ${EINA_INCLUDE_DIRS}
  ${X11_INCLUDE_DIRS}
  ${X11_Xrender_INCLUDE_PATH}
)

add_definitions(-include cstddef)

set(LIBS ${LIBS}
  ${EVAS_LIBRARIES}
  ${EINA_LIBRARIES}
  ${X11_LIBRARIES}
  ${X11_Xrender_LIB}
) 

add_executable(Wbar
  Wbar.cc
  OptionParser.cc
  ConfigReader.cc
  CanvasEngine.cc
  LayoutStrategy.cc
  Geometry.cc
  Xwindow.cc
)

target_link_libraries(Wbar ${LIBS})
