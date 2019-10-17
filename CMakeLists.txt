cmake_minimum_required(VERSION 3.11)
project(negascan)

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-p=${CMAKE_BINARY_DIR},-config=")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Logging and args
find_package(glog REQUIRED)
find_package(gflags REQUIRED)

set(IMGUI_ROOT ${CMAKE_SOURCE_DIR}/vendors/imgui)

add_subdirectory(src)

# include_directories(${TIFF_INCLUDE_DIR})
#
# # QT5
# find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
# include_directories(${Qt5Widgets_INCLUDES})
# add_definitions(${Qt5Widgets_DEFINITIONS})
# set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# qt5_wrap_cpp(WRAPPED_HEADERS
#   include/PreviewWidget.h
#   include/ControlsWidget.h
#   include/RawProcessor.h
#   include/RGBControls.h
#   include/HistogramWidget.h
# )
#
# qt5_add_resources(RESOURCES
#   resources/manifest.qrc
# )
#
# message("resources" ${RESOURCES})
#
# set(SOURCES
# )
#
# add_executable(main ${SOURCES} ${WRAPPED_HEADERS} ${RESOURCES})
#
# qt5_use_modules(main Widgets Core)
#
