cmake_minimum_required(VERSION 3.5)
project(examples LANGUAGES CXX VERSION 0.1)


# Framebuffers Example
add_executable(example_framebuffers "framebuffers.cpp")
target_link_libraries(example_framebuffers giygas giygasutil)

set_target_properties(example_framebuffers
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
)

# Compile content
file(GLOB_RECURSE GIYGAS_EXAMPLE_CONTENT_FILES ${CMAKE_CURRENT_SOURCE_DIR}
    "./content/*"
)

add_custom_target(example_content ALL)
foreach(CURRENT_CONTENT_FILE ${WIZKID_CONTENT_FILES})
    add_custom_command(TARGET example_content
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_CONTENT_FILE}"
            "$<TARGET_FILE_DIR:example_framebuffers>/${CURRENT_CONTENT_FILE}"
    )
endforeach()
