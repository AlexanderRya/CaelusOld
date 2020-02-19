#ifndef CAELUS_TYPES_HPP
#define CAELUS_TYPES_HPP

#include "glm/glm.hpp"

using i8 = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using f32 = float;
using f64 = double;

using isize = i64;
using usize = u64;

namespace caelus::core::types {
    struct Vertex {
        glm::vec3 pos;
        glm::vec2 tx_coords;
    };

    struct TransformUBO {
        glm::mat4 pv_mat;
        glm::mat4 model;
    };
}

#endif //CAELUS_TYPES_HPP
