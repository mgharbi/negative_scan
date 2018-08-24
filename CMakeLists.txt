cmake_minimum_required(VERSION 3.11)
project(negascan)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Libraw
find_package(LibRaw REQUIRED)
include_directories(${LIBRAW_INCLUDE_DIR})
add_definitions(${LIBRAW_DEFINITIONS})

find_package(JPEG REQUIRED)
find_package(TIFF REQUIRED)
find_package(JASPER REQUIRED)
find_package(LCMS REQUIRED)

include_directories(${TIFF_INCLUDE_DIR})

# QT5
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# Halide
set(HALIDE_DISTRIB_DIR ${CMAKE_CURRENT_LIST_DIR}/vendors/halide)
set(HALIDE_DISTRIB_USE_STATIC_LIBRARY TRUE)  # issue with dylib
include("${HALIDE_DISTRIB_DIR}/halide.cmake")
include_directories(${HALIDE_INCLUDE_DIR})

# link_directories(${HALIDE_DISTRIB_DIR}/bin)

include_directories(include)
qt5_wrap_cpp(WRAPPED_HEADERS
  include/PreviewWidget.h
  include/ControlsWidget.h
  include/RawProcessor.h
  include/RGBControls.h
)

qt5_add_resources(RESOURCES
  resources/manifest.qrc
)

message("resources" ${RESOURCES})

set(SOURCES
 src/main.cpp
 src/MainWindow.cpp
 src/ControlsWidget.cpp
 src/RGBControls.cpp
 src/PreviewWidget.cpp
 src/RawProcessor.cpp
 src/HistogramWidget.cpp
)

halide_library(subsample SRCS src/hl/subsample.cpp)
halide_library(histogram SRCS src/hl/histogram.cpp)
halide_library(minmax SRCS src/hl/minmax.cpp)
halide_library(invert_negative SRCS src/hl/invert_negative.cpp)
halide_library(invert_negative_bw SRCS src/hl/invert_negative.cpp
               GENERATOR_ARGS bw=true
               GENERATOR_NAME invert_negative)
set(NEGASCAN_HL_LIBS
  subsample
  histogram
  minmax
  invert_negative
  invert_negative_bw
)

# message(flags ${CMAKE_CXX_FLAGS})
# set(${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS} -fvisibility=default)  # Halide complains otherwise

add_executable(main ${SOURCES} ${WRAPPED_HEADERS} ${RESOURCES})

qt5_use_modules(main Widgets Core)

target_link_libraries(main ${LIBRAW_LIBRARIES})
target_link_libraries(main ${JPEG_LIBRARIES})
target_link_libraries(main ${TIFF_LIBRARIES})
target_link_libraries(main ${JASPER_LIBRARIES})
target_link_libraries(main ${LCMS_LIBRARIES})
target_link_libraries(main ${NEGASCAN_HL_LIBS})
