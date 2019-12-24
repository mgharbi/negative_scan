add_subdirectory(mgl)

# Halide
set(HALIDE_DISTRIB_DIR ${CMAKE_SOURCE_DIR}/vendors/halide)
set(HALIDE_DISTRIB_USE_STATIC_LIBRARY TRUE)  # issue with dylib
include("${HALIDE_DISTRIB_DIR}/halide.cmake")
# link_directories(${HALIDE_DISTRIB_DIR}/bin)

halide_library(subsample SRCS hl/subsample.cpp)
halide_library(histogram SRCS hl/histogram.cpp)
halide_library(minmax SRCS hl/minmax.cpp)
halide_library(invert_negative SRCS hl/invert_negative.cpp)
halide_library(invert_negative_bw SRCS hl/invert_negative.cpp
    GENERATOR_ARGS bw=true
    GENERATOR_NAME invert_negative)


include_directories(${HALIDE_INCLUDE_DIR})
include_directories(${IMGUI_ROOT}/examples)
include_directories(${IMGUI_ROOT})
include_directories(${CMAKE_SOURCE_DIR}/include)


set(NEGASCAN_HL_LIBS
    subsample
    histogram
    minmax
    invert_negative
    invert_negative_bw)

set(SRCS
    main.cpp
    negascan.cpp
    raw_processor.cpp
    image.cpp
# MainWindow.cpp
# ControlsWidget.cpp
# RGBControls.cpp
# PreviewWidget.cpp
# HistogramWidget.cpp
)

# Libraw
find_package(JPEG REQUIRED)
find_package(TIFF REQUIRED)
find_package(JASPER REQUIRED)
find_package(LCMS REQUIRED)
find_package(LibRaw REQUIRED)
include_directories(${LIBRAW_INCLUDE_DIR})
add_definitions(${LIBRAW_DEFINITIONS})


add_executable(negascan ${SRCS})
target_link_libraries(negascan 
    mgl
    glog::glog
    gflags
    ${LIBRAW_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${TIFF_LIBRARIES}
    ${JASPER_LIBRARIES}
    ${LCMS_LIBRARIES}
    ${LIBRAW_LIBRARIES}
    ${NEGASCAN_HL_LIBS}
)
