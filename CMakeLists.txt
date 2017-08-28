cmake_minimum_required(VERSION 3.5)
project(giygas LANGUAGES CXX VERSION 0.1)

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

enable_testing()
find_package(GTest REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Library target
file(GLOB_RECURSE GIYGAS_SOURCE_FILES "./src/*" "./include/*")
file(GLOB_RECURSE GIYGAS_TEST_SOURCE_FILES "./test/*")
add_library(giygas SHARED ${GIYGAS_SOURCE_FILES})

generate_export_header(giygas
    BASE_NAME GIYGAS
    EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/include/giygas/export.h"
)

set_target_properties(giygas
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
target_link_libraries(giygas
    glad::glad glfw
)

# Test executable target
add_executable(giygas_test ${GIYGAS_TEST_SOURCE_FILES})
target_link_libraries(giygas_test GTest::GTest GTest::Main giygas)

# Tests
add_test(GiygasTests giygas_test)


# Install
set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")
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
    INCLUDES DESTINATION "${include_install_dir}"
)
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include"
    DESTINATION ${CMAKE_INSTALL_PREFIX}
)
install (
    DIRECTORY "${PROJECT_BINARY_DIR}/include"
    DESTINATION ${CMAKE_INSTALL_PREFIX}
)
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)
install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)


