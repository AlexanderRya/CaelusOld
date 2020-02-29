#version 460 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec2 itx_coords;

/*layout (set = 0, binding = 0) uniform CameraData {
    mat4 pv_matrix;
} camera_data;*/

layout (std430, set = 0, binding = 1) buffer readonly Instances {
    mat4 transforms[];
} instances;

void main() {
    gl_Position = /*camera_data.pv_matrix * */instances.transforms[gl_InstanceIndex] * vec4(ipos, 1.0);
}
