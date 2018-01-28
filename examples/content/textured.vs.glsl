#version 330

layout(location = 0)in vec3 pos;
layout(location = 1)in vec2 vertexUV;

out vec2 uv;

uniform mat4 modelView;
uniform mat4 worldView;

void main()
{
    gl_Position = worldView * modelView * vec4(pos, 1);
    uv = vertexUV;
}
