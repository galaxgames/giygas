#version 330

layout(location = 0)in vec3 pos;
layout(location = 2)in vec3 vertexColor;

out vec4 color;

uniform mat4 modelView;
uniform mat4 worldView;

void main()
{
    gl_Position = worldView * modelView * vec4(pos, 1);
    color = vec4(vertexColor, 1);
}
