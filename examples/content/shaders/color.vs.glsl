#version 450

layout(location = 0)in vec3 pos;
layout(location = 2)in vec3 vertexColor;

layout(location = 0)out vec4 color;

layout(push_constant) uniform UserData {
    mat4 modelView;
    mat4 worldView;
} userdata;

void main()
{
    gl_Position = userdata.worldView * userdata.modelView * vec4(pos, 1);
    color = vec4(vertexColor, 1);
}
