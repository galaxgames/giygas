#version 450

layout(location = 0)in vec3 pos;
layout(location = 1)in vec3 vertexColor;

layout(location = 0)out vec4 color;

layout(binding = 0) uniform UserData {
    mat4 model_tf;
    mat4 view_tf;
    mat4 projection_tf;
} userdata;


void main() {
    gl_Position = userdata.projection_tf * userdata.view_tf * userdata.model_tf * vec4(pos, 1);
    color = vec4(vertexColor, 1);
}
