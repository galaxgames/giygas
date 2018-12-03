cmake_minimum_required(VERSION 3.5)
project(giygasfile LANGUAGES CXX VERSION 0.1)

include(GenerateExportHeader)

file(GLOB_RECURSE GIYGASFILE_SOURCE_FILES
    "./src/*.cpp"
    "./include/giygasfile/*.hpp"
)

add_library(giygasfile ${GIYGASFILE_SOURCE_FILES})

generate_export_header(giygasfile
    BASE_NAME GIYGASFILE
    EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/include/giygasfile/export.h"
)

set_target_properties(giygasfile
    PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_VISIBILITY_PRESET hidden
)

target_include_directories(giygasfile
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    INTERFACE
        $<INSTALL_INTERFACE:include/giygasfile>
)

target_link_libraries(giygasfile giygas)

#
# Install
#
install(
    TARGETS giygasfile
    EXPORT giygasfileTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)
install(
    EXPORT giygasfileTargets
    NAMESPACE "giygas::"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/giygasfile"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/giygasfile"
)
install (
    DIRECTORY "${PROJECT_BINARY_DIR}/include/giygasfile"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/giygasfile"
)
