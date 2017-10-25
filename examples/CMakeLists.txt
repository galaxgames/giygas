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