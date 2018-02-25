#version 450

layout(location = 0)in vec4 vertex_pos;
layout(location = 1)in vec2 vertex_uv;

layout(location = 0)out vec2 uv;

void main() {
    gl_Position = vertex_pos;
    uv = vertex_uv;
}
