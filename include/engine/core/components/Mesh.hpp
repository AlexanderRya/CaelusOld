#ifndef CAELUS_MESH_HPP
#define CAELUS_MESH_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"
#include <vector>

namespace caelus::core::components {
    struct Mesh {
        usize pipeline_idx{};
        usize vertex_buffer_idx{};

        usize vertex_count{};
        usize instances_count{};
        //!TODO: Textures.
    };
} // namespace caelus::core::components

#endif //CAELUS_MESH_HPP
