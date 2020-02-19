#version 460 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec2 itx_coords;

layout (location = 6) in mat4 instances;

void main() {
    gl_Position = instances * vec4(ipos, 1.0);
}
