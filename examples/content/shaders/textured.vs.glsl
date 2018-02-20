#version 450

layout(location = 0)in vec3 pos;
layout(location = 2)in vec2 vertexUV;

layout(location = 0)out vec2 uv;

layout(binding = 0) uniform UserData {
    mat4 model_tf;
    mat4 view_tf;
    mat4 projection_tf;
} userdata;


void main() {
    gl_Position = userdata.projection_tf * userdata.view_tf * userdata.model_tf * vec4(pos, 1);
    uv = vertexUV;
}
