#version 450

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragment_color;

layout(binding = 0) uniform sampler2D tex;

void main() {
    fragment_color = texture(tex, uv);
}
