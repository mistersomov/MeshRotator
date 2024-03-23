cmake_minimum_required(VERSION 3.22.1)

project("native_testing")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (NOT TARGET native_testing)
    add_subdirectory(${NDK_HELPER_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libndk_helper")

    find_package(googletest REQUIRED CONFIG)
    find_package(junit-gtest REQUIRED CONFIG)

    #glm
    set(GLM_DIR ${CMAKE_ANDROID_NDK}/sources/glm)
    add_library(glm SHARED IMPORTED)
    include_directories(${GLM_DIR})
    set_target_properties(glm PROPERTIES IMPORTED_LOCATION ${GLM_DIR}/build/${ANDROID_ABI}/glm/libglm.so)

    add_library(native_testing SHARED
        src/main/cpp/ndk-helper/ModelTest.cpp
    )

    target_include_directories(native_testing PRIVATE
        ${ndk_helper_SOURCE_DIR}/src/main/cpp
    )

    target_link_libraries(native_testing
        ndk_helper

        googletest::gmock
        googletest::gtest
        junit-gtest::junit-gtest

        #glm
        glm

        EGL
        GLESv3

        android
        log
    )

    target_compile_options(native_testing PRIVATE
        -Wall
        -Wextra
        -frtti
    )

    set_target_properties(native_testing PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}"
    )
ENDIF (NOT TARGET native_testing)