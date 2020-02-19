#ifndef CAELUS_MESH_HPP
#define CAELUS_MESH_HPP

#include "engine/core/components/buffers/InstanceBuffer.hpp"
#include "engine/core/components/buffers/VertexBuffer.hpp"
#include "engine/core/Types.hpp"
#include <vector>

namespace caelus::core::components {
    struct Mesh {
        std::vector<types::Vertex> vertices;
        std::vector<glm::mat4> instances;

        u64 pipeline_id;
        buffers::VertexBuffer vertex_buffer;
        buffers::InstanceBuffer instance_buffer;
        //!TODO: Textures.
    };
} // namespace caelus::core::components

#endif //CAELUS_MESH_HPP
