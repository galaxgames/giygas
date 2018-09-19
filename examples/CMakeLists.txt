cmake_minimum_required(VERSION 3.5)
project(examples LANGUAGES CXX VERSION 0.1)

#
# Content Preprocessor Functions
# TODO: Make this functionality part of a CMake file that can be included by projects
#

find_program(GLSLANG_VALIDATOR glslangValidator)

function(add_shader_compile_commands shaderstage output_list infile outfile)
    string(REGEX MATCH "^(.*).glsl" dummy "${outfile}")
    set(spv_filename "${CMAKE_MATCH_1}.spv")
    get_filename_component(dest_dir "${outfile}" DIRECTORY)
    add_custom_command(
            OUTPUT "${spv_filename}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${dest_dir}"
            COMMAND "${GLSLANG_VALIDATOR}"
            -S "${shaderstage}"
            -o "${spv_filename}"
            -V
            "${infile}"
            DEPENDS "${infile}"
    )
    list(APPEND output_list ${spv_filename})
    set(shader_output_files "${output_list}" PARENT_SCOPE)
endfunction()

function(add_content_commands content_source_files generated_content_files_var)
    set (generated_content_files "")
    foreach(current_content_file ${content_source_files})
        get_filename_component(extension "${current_content_file}" EXT)
        set(input_file_path "${CMAKE_CURRENT_SOURCE_DIR}/${current_content_file}")
        set(output_file_path "${CMAKE_CURRENT_BINARY_DIR}/${current_content_file}")
        if(extension STREQUAL ".vs.glsl")
            add_shader_compile_commands(vert "${generated_content_files}" "${input_file_path}" "${output_file_path}")
            list(APPEND generated_content_files ${shader_output_files})
        elseif(extension STREQUAL ".fs.glsl")
            add_shader_compile_commands(frag "${generated_content_files}" "${input_file_path}" "${output_file_path}")
            list(APPEND generated_content_files ${shader_output_files})
        else()
            add_custom_command(
                OUTPUT "${output_file_path}"
                COMMAND ${CMAKE_COMMAND} -E copy "${input_file_path}" "${output_file_path}"
                DEPENDS "${input_file_path}"
            )
            list(APPEND generated_content_files "${output_file_path}")
        endif()
    endforeach()
    set("${generated_content_files_var}" "${generated_content_files}" PARENT_SCOPE)
endfunction()

#
# End content preprocessor functions
#

file(GLOB_RECURSE GIYGAS_EXAMPLE_CONTENT_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "./content/*"
)
add_content_commands("${GIYGAS_EXAMPLE_CONTENT_SOURCE_FILES}" GIYGAS_EXAMPLE_CONTENT_GENERATED_CONTENT_FILES)
add_custom_target(example_content DEPENDS ${GIYGAS_EXAMPLE_CONTENT_GENERATED_CONTENT_FILES} SOURCES ${GIYGAS_EXAMPLE_CONTENT_SOURCE_FILES})

function(add_example name source_file)
    add_executable("${name}" "example_common.hpp" "example_common.cpp" "${source_file}")
    target_link_libraries("${name}" giygas giygasutil)
    add_dependencies("${name}" example_content)
    set_target_properties(
        "${name}"
        PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED ON
    )
endfunction()

#
# All example executables are added below:
#
add_example(example_framebuffers "framebuffers.cpp")
add_example(example_triangle "triangle.cpp")
add_example(example_texture "texture.cpp")
add_example(example_input "input.cpp")