#version 460 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec2 itx_coords;

layout (location = 6) in vec3 instances;


void main() {
    gl_Position = vec4(ipos + instances, 1.0);
}
