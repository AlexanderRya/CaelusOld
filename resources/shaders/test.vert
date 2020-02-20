#version 460 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec2 itx_coords;

layout (location = 6) in mat4 instances;

/*layout (set = 0, binding = 0) uniform Matrices {
    mat4 pv_matrix;
    mat4 model;
} matrices;*/

void main() {
    gl_Position = /*matrices.pv_matrix * matrices.model * */instances * vec4(ipos, 1.0);
}
