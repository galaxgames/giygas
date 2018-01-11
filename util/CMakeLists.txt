cmake_minimum_required(VERSION 3.5)
project(giygasutil LANGUAGES CXX VERSION 0.1)

include(GenerateExportHeader)

file(GLOB_RECURSE GIYGASUTIL_SOURCE_FILES
    "./src/*.cpp"
    "./src/*.hpp"
    "./include/giygasutil/*.hpp"
)

add_library(giygasutil ${GIYGASUTIL_SOURCE_FILES})

generate_export_header(giygasutil
    BASE_NAME GIYGASUTIL
    EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/include/giygasutil/export.h"
)

set_target_properties(giygasutil
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        CXX_VISIBILITY_PRESET hidden
)

target_include_directories(giygasutil
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    INTERFACE
        $<INSTALL_INTERFACE:include/giygasutil>
)

target_link_libraries(giygasutil giygas)

#
# Install
#
install(
    TARGETS giygasutil
    EXPORT giygasutilTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)
install(
    EXPORT giygasutilTargets
    NAMESPACE "giygas::"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/giygasutil"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
)
install (
    DIRECTORY "${PROJECT_BINARY_DIR}/include"
    DESTINATION ${CMAKE_INSTALL_PREFIX}
)
