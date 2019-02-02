cmake_minimum_required(VERSION 3.5)
project(giygas_core LANGUAGES CXX VERSION 0.1)

include(GenerateExportHeader)

enable_testing()
find_package(glad CONFIG)
find_package(Vulkan)
find_package(glfw3 CONFIG REQUIRED)
find_package(GTest CONFIG)

set(giygas_with_opengl_default OFF)
set(giygas_with_vulkan_default OFF)
if (glad_FOUND)
    set(giygas_with_opengl_default ON)
endif()
if (Vulkan_FOUND)
    set(giygas_with_vulkan_default ON)
endif()

option(GIYGAS_WITH_OPENGL "Include OpenGL Renderer Implementation" "${giygas_with_opengl_default}")
option(GIYGAS_WITH_VULKAN "Include Vulkan Renderer Implementation" "${giygas_with_vulkan_default}")

file(GLOB GIYGAS_SOURCE_FILES
    "./src/*.cpp"
    "./src/*.hpp"
    "./src/validation/*.cpp"
    "./src/validation/*.hpp"
    "./include/giygas/*.hpp"
    "./include/giygas/validation/*.hpp"
)
file(GLOB_RECURSE GIYGAS_TEST_SOURCE_FILES
    "./test/core/*.cpp"
    "./test/core/*.hpp"
)
file(GLOB_RECURSE GIYGAS_OPENGL_SOURCE_FILES
    "./src/gl/*.cpp"
    "./src/gl/*.hpp"
)
file(GLOB_RECURSE GIYGAS_TEST_OPENGL_SOURCE_FILES
    "./test/src/gl/*.cpp"
    "./test/src/gl/*.hpp"
)
file(GLOB_RECURSE GIYGAS_VULKAN_SOURCE_FILES
    "./src/vulkan/*.cpp"
    "./src/vulkan/*.hpp"
)

if (GIYGAS_WITH_OPENGL)
    list(APPEND GIYGAS_SOURCE_FILES ${GIYGAS_OPENGL_SOURCE_FILES})
    list(APPEND GIYGAS_TEST_SOURCE_FILES ${GIYGAS_TEST_OPENGL_SOURCE_FILES})
endif()
if (GIYGAS_WITH_VULKAN)
    list(APPEND GIYGAS_SOURCE_FILES ${GIYGAS_VULKAN_SOURCE_FILES})
endif()

add_library(giygas ${GIYGAS_SOURCE_FILES})
if (GTest_FOUND)
    add_executable(giygas_test ${GIYGAS_TEST_SOURCE_FILES})
endif()

set_target_properties(giygas
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
)

if (GTest_FOUND)
    set_target_properties(giygas_test
        PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED ON
    )
endif()

set_target_properties(giygas PROPERTIES CXX_VISIBILITY_PRESET hidden)

generate_export_header(giygas
    BASE_NAME GIYGAS
    EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/include/giygas/export.h"
)

target_include_directories(giygas
    PRIVATE
        # $<TARGET_PROPERTY:glad::glad,INTERFACE_INCLUDE_DIRECTORIES>
        # $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
        # $<TARGET_PROPERTY:Vulkan::Vulkan,INTERFACE_INCLUDE_DIRECTORIES>
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include/giygas>
)

configure_file("config.hpp.in" "include/giygas/config.hpp")

set(giygas_link_libraries glfw)
if (GIYGAS_WITH_VULKAN)
    list(APPEND giygas_link_libraries Vulkan::Vulkan)
endif()
if (GIYGAS_WITH_OPENGL)
    list(APPEND giygas_link_libraries glad::glad)
endif()

target_link_libraries(giygas PRIVATE ${giygas_link_libraries})

if (GTest_FOUND)
    target_link_libraries(giygas_test
        GTest::gtest GTest::gtest_main GTest::gmock giygas
    )

    add_test(GiygasTests giygas_test)
endif()

#
# Install
#
install(
    TARGETS giygas
    EXPORT giygasTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)
install(
    EXPORT giygasTargets
    NAMESPACE "giygas::"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)

# Copy include files to (prefix)/include/giygas/giygas
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/giygas"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/giygas"
)
install (
    DIRECTORY "${PROJECT_BINARY_DIR}/include/giygas"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/giygas"
)
