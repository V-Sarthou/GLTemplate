cmake_minimum_required(VERSION 3.12)

project(MyGLProject VERSION 0.1.0 LANGUAGES C)

include(GNUInstallDirs)
include(FetchContent)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Link with MT when using MSVC
if(MSVC)
  set(CompilerFlags
      CMAKE_C_FLAGS
      CMAKE_C_FLAGS_DEBUG
      CMAKE_C_FLAGS_RELEASE
      )
  foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endforeach()
endif()

set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Add GLAD
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG        master
)
FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
  FetchContent_Populate(glad)
endif()
set(GLAD_API "gles2=2.0" CACHE STRING "")
set(GLAD_PROFILE "core" CACHE STRING "")
set(GLAD_EXPORT OFF CACHE BOOL "" FORCE)
add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})

# Add GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        master
)
FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
  FetchContent_Populate(glfw)
endif()
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})

if(${GLAD_API} MATCHES "gles")
  add_definitions(-DGLAD_GLES)
endif()

add_subdirectory(src)
