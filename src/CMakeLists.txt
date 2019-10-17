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
    RawProcessor.cpp
# MainWindow.cpp
# ControlsWidget.cpp
# RGBControls.cpp
# PreviewWidget.cpp
# HistogramWidget.cpp
)

add_executable(main ${SRCS})
target_link_libraries(main 
    mgl
    glog::glog
    gflags
    # ${LIBRAW_LIBRARIES}
)
# target_link_libraries(main ${LIBRAW_LIBRARIES})
# target_link_libraries(main ${JPEG_LIBRARIES})
# target_link_libraries(main ${TIFF_LIBRARIES})
# target_link_libraries(main ${JASPER_LIBRARIES})
# target_link_libraries(main ${LCMS_LIBRARIES})
# target_link_libraries(main ${NEGASCAN_HL_LIBS})
