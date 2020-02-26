#version 460 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec2 itx_coords;
/*
layout (set = 0, binding = 0) uniform Matrices0 {
    mat4 model;
} mat0;

layout (set = 1, binding = 0) uniform Matrices1 {
    mat4 pv_matrix;
} mat1;*/

layout (std430, set = 0, binding = 1) buffer readonly Instances {
    mat4 transforms[];
} instances;

void main() {
    gl_Position = /*mat1.pv_matrix * mat0.model * */instances.transforms[gl_InstanceIndex] * vec4(ipos, 1.0);
}
