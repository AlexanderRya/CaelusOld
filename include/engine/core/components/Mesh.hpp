#ifndef CAELUS_MESH_HPP
#define CAELUS_MESH_HPP

#include "engine/core/components/buffers/VertexBuffer.hpp"
#include "engine/core/Types.hpp"
#include <vector>

namespace caelus::core::components {
    struct Mesh {
        vk::Pipeline pipeline;

        vk::Buffer vertex_buffer;
        vk::DeviceMemory vertex_memory;

        usize vertex_count;
        //!TODO: Textures.
    };
} // namespace caelus::core::components

#endif //CAELUS_MESH_HPP
