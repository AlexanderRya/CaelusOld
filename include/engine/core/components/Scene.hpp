#ifndef CAELUS_SCENE_HPP
#define CAELUS_SCENE_HPP

#include <vector>
#include "engine/core/Types.hpp"

namespace caelus::core::vulkan {
    struct Pipeline;
} // namespace caelus::core::vulkan

namespace caelus::core::components {
    struct Mesh;

    namespace buffers {
        struct VertexBuffer;
    } // namespace caelus::core::components::buffers

    struct Scene {
        std::vector<Mesh> meshes;
        std::vector<vulkan::Pipeline> pipelines;
        std::vector<buffers::VertexBuffer> vertex_buffers;
    };
} // namespace caelus::core::components

#endif //CAELUS_SCENE_HPP
