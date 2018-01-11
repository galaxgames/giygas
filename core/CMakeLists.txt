cmake_minimum_required(VERSION 3.5)
project(giygas_core LANGUAGES CXX VERSION 0.1)

include(GenerateExportHeader)

enable_testing()
find_package(GTest REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_library(GMOCK_LIBRARY NAMES gmock)

file(GLOB_RECURSE GIYGAS_SOURCE_FILES
    "./src/*.cpp"
    "./src/*.hpp"
    "./include/giygas/*.hpp"
)
file(GLOB_RECURSE GIYGAS_TEST_SOURCE_FILES
    "./test/*.cpp"
    "./test/*.hpp"
)

add_library(giygas_objects OBJECT ${GIYGAS_SOURCE_FILES})
add_library(giygas_internal STATIC $<TARGET_OBJECTS:giygas_objects>)
add_library(giygas $<TARGET_OBJECTS:giygas_objects>)
add_executable(giygas_test ${GIYGAS_TEST_SOURCE_FILES})

set_target_properties(giygas_objects giygas_test
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
)
set_target_properties(giygas giygas_objects giygas_internal PROPERTIES CXX_VISIBILITY_PRESET hidden)

generate_export_header(giygas
    BASE_NAME GIYGAS
    EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/include/giygas/export.h"
)

target_include_directories(giygas_objects
    PRIVATE
        $<TARGET_PROPERTY:glad::glad,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)
target_include_directories(giygas
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include/giygas>
)
target_include_directories(giygas_internal
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)
#target_include_directories(giygas_test
#    PRIVATE
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
#)

target_link_libraries(giygas glad::glad glfw)
target_link_libraries(giygas_internal glad::glad glfw)
target_link_libraries(giygas_test
    GTest::GTest GTest::Main ${GMOCK_LIBRARY} giygas_internal
)

add_test(GiygasTests giygas_test)

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
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/giygas"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
)
install (
    DIRECTORY "${PROJECT_BINARY_DIR}/include"
    DESTINATION ${CMAKE_INSTALL_PREFIX}
)
