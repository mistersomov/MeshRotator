cmake_minimum_required(VERSION 3.22.1)

project("meshrotator")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include the native part of NDKHelper
add_subdirectory(${NDK_HELPER_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libndk_helper")

#gameActivity
find_package(game-activity REQUIRED CONFIG)

#glm
set(GLM_DIR ${CMAKE_ANDROID_NDK}/sources/glm)
add_library(glm SHARED IMPORTED)
include_directories(${GLM_DIR})
set_target_properties(glm PROPERTIES IMPORTED_LOCATION ${GLM_DIR}/build/${ANDROID_ABI}/glm/libglm.so)

#assimp
set(ASSIMP_DIR ${CMAKE_ANDROID_NDK}/sources/assimp)
set(ASSIMP_LIBRARY_DIRS "${ASSIMP_DIR}/build/${ANDROID_ABI}/code")
set(ASSIMP_INCLUDE_DIRS "${ASSIMP_DIR}/include" "${ASSIMP_DIR}/build/${ANDROID_ABI}/include")
add_library(assimp SHARED IMPORTED)
include_directories(${ASSIMP_INCLUDE_DIRS})
link_directories(${ASSIMP_LIBRARY_DIRS})
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${ASSIMP_DIR}/build/${ANDROID_ABI}/bin/libassimp.so)

add_library(meshrotator SHARED
    src/main/cpp/Renderer.cpp
    src/main/cpp/main.cpp

    src/main/cpp/scene/Scene.cpp
    src/main/cpp/scene/MainScene.cpp
    src/main/cpp/scene/Skybox.cpp
)
target_include_directories(meshrotator PRIVATE
    ${ndk_helper_SOURCE_DIR}/src/main/cpp
)
target_link_libraries(meshrotator
    ndk_helper

    #assimp
    assimp

    #glm
    glm

    # The game activity
    game-activity::game-activity_static

    EGL
    GLESv3

    android
    log
)