project(TheUnchartedForest)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/resources
  DESTINATION ${CMAKE_BINARY_DIR}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SMDSRC
  src/smd/smdloader.cpp
)

set(SMDHDR
  include/smd/smdloader.h
)

set(SRC
  src/camera.cpp
  src/console.cpp
  src/gui.cpp
  src/guibar.cpp
  src/guielement.cpp
  src/main.cpp
  src/mesh.cpp
  src/sdffont.cpp
  src/shader.cpp
  src/texture.cpp
  src/tuf.cpp
  src/utils.cpp
)

set(HDR
  include/camera.h
  include/console.h
  include/gui.h
  include/guibar.h
  include/guielement.h
  include/mesh.h
  include/sdffont.h
  include/shader.h
  include/tuf.h
  include/texture.h
  include/utils.h
)

set(EXTRA_LIBS
)

add_library(
    smdloader
    SHARED
    ${SMDSRC}
    ${SMDHDR}
)

add_executable(
  tuf
  ${SRC}
  ${HDR}
)

target_link_libraries(
  tuf
  smdloader
  m
  ${OPENGL_LIBRARIES}
  ${SDL2_LIBRARY}
  ${PNG_LIBRARY}
  ${EXTRA_LIBS}
)
