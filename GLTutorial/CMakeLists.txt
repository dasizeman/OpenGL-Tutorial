cmake_minimum_required(VERSION 2.6)
project(GLTutorial)

# Try to locate OpenGL
find_package(OpenGL)
if(NOT OPENGL_FOUND)
    message(fatal_error "Couldn't find OpenGL on this system")
endif()

set(SRC_ROOT ${GLTutorial_SOURCE_DIR}/src)

# Set platform-dependent library paths.
set(LIBS_ROOT ${GLTutorial_SOURCE_DIR}/libs)
if(WIN32)
    set(PLATFORM_DIR win32)
    set(GLEW_LIB_NAME glew32s.lib)
    set(GLFW_LIB_NAME glfw3.lib)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT
    /NODEFAULTLIB:LIBCMT")
    set(GLFW_LIBS glfw3)
else()
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_CXX_FLAGS -std=c++11)
    set(PLATFORM_DIR linux)
    set(GLEW_LIB_NAME libglew32.a)
    set(GLFW_LIB_NAME libglfw3.a)
    # Found these by running pkg-config --static --libs glfw3 after installing
    # glfw3 from source
    set(GLFW_LIBS glfw3 rt Xrandr Xinerama Xi Xcursor m dl Xrender drm Xdamage X11-xcb xcb-glx xcb-dri2 xcb-dri3 xcb-present xcb-sync xshmfence Xxf86vm Xfixes Xext X11 pthread xcb Xau Xdmcp)
endif()


set(GLEW_ROOT ${LIBS_ROOT}/${PLATFORM_DIR}/glew-1.13.0)
set(GLFW_ROOT ${LIBS_ROOT}/${PLATFORM_DIR}/glfw-3.1.2)

# Set extra include directories
include_directories(${GLEW_ROOT}/include)
include_directories(${GLFW_ROOT}/include)

# Add libraries
add_library(glew32 STATIC IMPORTED)
    set_target_properties(glew32 PROPERTIES IMPORTED_LOCATION
        ${GLEW_ROOT}/lib/${GLEW_LIB_NAME})
add_library(glfw3 STATIC IMPORTED)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION
        ${GLFW_ROOT}/lib/${GLFW_LIB_NAME})
# Add the executable

file(GLOB_RECURSE SRC_FILES ${SRC_ROOT}/*.cpp)
add_executable(GLTutorial ${SRC_ROOT}/tutorial_drawing.cpp
    ${SRC_ROOT}/GLUtils.cpp)

# Link the libraries
target_link_libraries(GLTutorial ${OPENGL_gl_LIBRARY} glew32 ${GLFW_LIBS})
