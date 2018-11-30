cmake_minimum_required(VERSION 3.5)
project(giygas VERSION 0.1)

include(CMakePackageConfigHelpers)

include("cmake/dew.cmake")
setup_dew(dew_dependencies "${CMAKE_CURRENT_SOURCE_DIR}/dewfile.json")

set(CONFIG_INSTALL_DIR "lib/cmake/giygas")

add_subdirectory(core)
add_subdirectory(util)
add_subdirectory(giygasfile)
add_subdirectory(examples)

add_dependencies(giygasfile giygas)
add_dependencies(giygasutil giygas)

set(VERSION_CONFIG_FILE "${PROJECT_BINARY_DIR}/giygasConfigVersion.cmake")
set(PROJECT_CONFIG_FILE "${PROJECT_BINARY_DIR}/giygasConfig.cmake")

write_basic_package_version_file(
    "${VERSION_CONFIG_FILE}"
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "Config.cmake.in"
    "${PROJECT_CONFIG_FILE}"
    INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}"
)

install(
    FILES "${PROJECT_CONFIG_FILE}" "${VERSION_CONFIG_FILE}"
    DESTINATION ${CONFIG_INSTALL_DIR}
)
