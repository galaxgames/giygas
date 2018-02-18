#version 450

layout(location = 0)in vec4 vertexPosition;
layout(location = 1)in vec4 vertexColor;

layout(location = 0)out vec4 color;

void main()
{
    gl_Position = vertexPosition;
    color = vertexColor;
}
