cmake_minimum_required(VERSION 3.11)
project(negascan)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# find_package(Qt5Core REQUIRED)
# find_package(Qt5Widgets REQUIRED)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

include_directories(include)
qt5_wrap_cpp(WRAPPED_HEADERS
  include/PreviewWidget.h
  include/ControlsWidget.h
)

qt5_add_resources(RESOURCES
  resources/shaders.qrc
)

message("resources" ${RESOURCES})

set(SOURCES
 src/main.cpp
 src/MainWindow.cpp
 src/ControlsWidget.cpp
 src/PreviewWidget.cpp
#  src/RawProcessor.cpp
 src/HistogramWidget.cpp
)


add_executable(main ${SOURCES} ${WRAPPED_HEADERS} ${RESOURCES})
qt5_use_modules(main Widgets Core)
