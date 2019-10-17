# set(MGL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/mgl)
# include_directories(${MGL_INCLUDE_DIR})

# OpenGL
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

set(SRCS
    application.cpp
    # ${MGL_INCLUDE_DIR}/application.h
)

# ImGUI
file(GLOB IMGUI_SRCS
    ${IMGUI_ROOT}/imgui.cpp
    ${IMGUI_ROOT}/imgui_demo.cpp
    ${IMGUI_ROOT}/imgui_draw.cpp
    ${IMGUI_ROOT}/imgui_widgets.cpp
    ${IMGUI_ROOT}/examples/imgui_impl_opengl3.cpp
    ${IMGUI_ROOT}/examples/imgui_impl_glfw.cpp
)

include_directories(${IMGUI_ROOT})
include_directories(${IMGUI_ROOT}/examples)

add_library(mgl ${IMGUI_SRCS} ${SRCS})
target_link_libraries(mgl 
    glfw 
    ${OPENGL_LIBRARIES} 
    ${GLEW_LIBRARIES}
    glm
)
