#version 450

layout(location = 0)in vec2 pos;

layout(push_constant) uniform UserData {
    mat4 tf;
} userdata;


void main() {
    vec4 out_pos = userdata.tf * vec4(pos, 0, 1);
    out_pos.y = -out_pos.y;
    gl_Position = out_pos;
}
