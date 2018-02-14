#version 450

layout(location = 0)in vec3 pos;
layout(location = 1)in vec2 vertexUV;

layout(location = 0)out vec2 uv;

layout(push_constant) uniform UserData {
    mat4 modelView;
    mat4 worldView;
} userdata;

void main()
{
    gl_Position = userdata.worldView * userdata.modelView * vec4(pos, 1);
    uv = vertexUV;
}
