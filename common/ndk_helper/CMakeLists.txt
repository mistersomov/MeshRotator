cmake_minimum_required(VERSION 3.22.1)

project(ndk_helper)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -frtti -Wall -Werror")

IF(NOT TARGET ndk_helper)
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

    add_library(ndk_helper STATIC
        src/main/cpp/utils/AndroidOut.cpp
        src/main/cpp/utils/Utility.cpp
        src/main/cpp/utils/AssimpUtility.cpp
        src/main/cpp/utils/GlmUtility.cpp

        src/main/cpp/manager/ModelManager.cpp
        src/main/cpp/manager/AssetManager.cpp
        src/main/cpp/manager/TimeManager.cpp
        src/main/cpp/manager/ShaderManager.cpp

        src/main/cpp/Mesh.cpp
        src/main/cpp/Model.cpp
        src/main/cpp/Shader.cpp

        src/main/cpp/core/Body.cpp
        src/main/cpp/core/SkinnedBody.cpp
        src/main/cpp/core/Bone.cpp
        src/main/cpp/core/Camera.cpp

        src/main/cpp/light/DirectionalLight.cpp
        src/main/cpp/light/PointLight.cpp
    )
    target_link_libraries(ndk_helper
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
    set_target_properties(ndk_helper PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}"
    )

ENDIF (NOT TARGET ndk_helper)