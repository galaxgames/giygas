cmake_minimum_required(VERSION 3.5)
project(giygas LANGUAGES CXX VERSION 0.1)

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

enable_testing()
find_package(GTest REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_library(GMOCK_LIBRARY NAMES gmock)

# Library target
file(GLOB_RECURSE GIYGAS_SOURCE_FILES "./src/*" "./include/*")
file(GLOB_RECURSE GIYGAS_TEST_SOURCE_FILES "./test/*")
add_library(giygas ${GIYGAS_SOURCE_FILES})
add_library(giygas_testable ${GIYGAS_SOURCE_FILES})
# Pre-define GIYGAS_TESTABLE_EXPORT so testable exports aren't exported for
# main target.
target_compile_definitions(giygas PRIVATE GIYGAS_TESTABLE_EXPORT)

generate_export_header(giygas
    BASE_NAME GIYGAS
    EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/include/giygas/export.h"
)

generate_export_header(giygas
    BASE_NAME GIYGAS_TESTABLE
    EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/include/giygas/testable_export.h"
)

set_target_properties(giygas
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
)
set_target_properties(giygas_testable
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
)


target_include_directories(giygas
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    PUBLIC
        $<INSTALL_INTERFACE:include/giygas>
)
target_include_directories(giygas_testable
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_link_libraries(giygas
    glad::glad glfw
)
target_link_libraries(giygas_testable
    glad::glad glfw
)

# Test executable target
add_executable(giygas_test ${GIYGAS_TEST_SOURCE_FILES})
set_target_properties(giygas_test
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
)
target_include_directories(giygas_test
    PRIVATE
        # TODO: Get these directly from giygas target
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)
target_link_libraries(giygas_test
    GTest::GTest GTest::Main ${GMOCK_LIBRARY} giygas_testable
)

# Tests
add_test(GiygasTests giygas_test)

# Install
set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(version_config "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Configure '${PROJECT_NAME}ConfigVersion.cmake'
# Note: PROJECT_VERSION is used as a VERSION
write_basic_package_version_file(
    "${version_config}"
    COMPATIBILITY SameMajorVersion
)

# Configure '${PROJECT_NAME}Config.cmake'
# Use variables:
#   * targets_export_name
configure_package_config_file(
    "Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

install(
    TARGETS giygas
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/giygas"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
)
install (
    DIRECTORY "${PROJECT_BINARY_DIR}/include"
    DESTINATION ${CMAKE_INSTALL_PREFIX}
)
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION ${config_install_dir}
)
install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)


