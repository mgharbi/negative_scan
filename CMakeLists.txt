cmake_minimum_required(VERSION 3.11)
project(negascan)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Libraw
find_package(LibRaw REQUIRED)
include_directories(${LIBRAW_INCLUDE_DIR})
add_definitions(${LIBRAW_DEFINITIONS})

find_package(JPEG REQUIRED)
find_package(JASPER REQUIRED)
find_package(LCMS REQUIRED)

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
)

qt5_add_resources(RESOURCES
  resources/manifest.qrc
)

message("resources" ${RESOURCES})

set(SOURCES
 src/main.cpp
 src/MainWindow.cpp
 src/ControlsWidget.cpp
 src/PreviewWidget.cpp
 src/RawProcessor.cpp
 src/HistogramWidget.cpp
)

halide_library(subsampler SRCS src/hl/subsampler_generator.cpp)
set(NEGASCAN_HL_LIBS
  subsampler
)

add_executable(main ${SOURCES} ${WRAPPED_HEADERS} ${RESOURCES})

qt5_use_modules(main Widgets Core)

target_link_libraries(main ${LIBRAW_LIBRARIES})
target_link_libraries(main ${JPEG_LIBRARIES})
target_link_libraries(main ${JASPER_LIBRARIES})
target_link_libraries(main ${LCMS_LIBRARIES})
target_link_libraries(main ${NEGASCAN_HL_LIBS})
