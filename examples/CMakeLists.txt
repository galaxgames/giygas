cmake_minimum_required(VERSION 3.5)
project(examples LANGUAGES CXX VERSION 0.1)


# Framebuffers Example
add_executable(example_framebuffers "example_common.cpp" "example_common.hpp" "framebuffers.cpp")
add_executable(example_triangle "example_common.cpp" "example_common.hpp" "triangle.cpp")
add_executable(example_texture "example_common.cpp" "example_common.hpp" "texture.cpp")
target_link_libraries(example_framebuffers giygas giygasutil)
target_link_libraries(example_triangle giygas giygasutil)
target_link_libraries(example_texture giygas giygasutil)

set_target_properties(example_framebuffers example_triangle example_texture
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
)

# Compile content
file(GLOB_RECURSE GIYGAS_EXAMPLE_CONTENT_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "./content/*"
)

function(add_shader_compile_commands shaderstage target infile outfile)
    string(REGEX MATCH "^(.*).glsl" dummy "${outfile}")
    set(spv_filename "${CMAKE_MATCH_1}.spv")
    get_filename_component(dest_dir "${outfile}" DIRECTORY)
    add_custom_command(TARGET "${target}" COMMAND "${CMAKE_COMMAND}" -E make_directory "${dest_dir}")
    add_custom_command(
            TARGET "${target}"
            COMMAND "glslangValidator"
            -S "${shaderstage}"
            -o "${spv_filename}"
            -V
            "${infile}"
    )
endfunction()

add_custom_target(example_content ALL DEPENDS ${GIYGAS_EXAMPLE_CONTENT_FILES})
foreach(current_content_file ${GIYGAS_EXAMPLE_CONTENT_FILES})
    get_filename_component(extension "${current_content_file}" EXT)
    set(input_file_path "${CMAKE_CURRENT_SOURCE_DIR}/${current_content_file}")
    set(output_file_path "${CMAKE_CURRENT_BINARY_DIR}/${current_content_file}")
    if(extension STREQUAL ".vs.glsl")
        add_shader_compile_commands(vert example_content "${input_file_path}" "${output_file_path}")
    elseif(extension STREQUAL ".fs.glsl")
        add_shader_compile_commands(frag example_content "${input_file_path}" "${output_file_path}")
    else()
        add_custom_command(TARGET example_content
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${input_file_path}" "${output_file_path}"
        )
    endif()
endforeach()

add_dependencies(example_framebuffers example_content)
add_dependencies(example_triangle example_content)
add_dependencies(example_texture example_content)
