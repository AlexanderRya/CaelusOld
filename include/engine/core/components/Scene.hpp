#ifndef CAELUS_SCENE_HPP
#define CAELUS_SCENE_HPP

#include "engine/core/components/buffers/GenericBuffer.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/vulkan/DescriptorSet.hpp"
#include "engine/core/vulkan/Pipeline.hpp"
#include "engine/core/Types.hpp"

#include <vector>

namespace caelus::core::vulkan {
    struct PipelineLayout;
} // namespace caelus::core::vulkan

namespace caelus::core::components {
    struct Mesh;

    namespace buffers {
        struct VertexBuffer;
    } // namespace caelus::core::components::buffers

    struct Scene {
        std::vector<Mesh> meshes;

        vulkan::Pipeline mesh_pipeline;
        std::vector<vulkan::PipelineLayout> layouts;
        vulkan::DescriptorSet camera_descriptor;

        std::vector<buffers::VertexBuffer> vertex_buffers;
        buffers::GenericBuffer<types::CameraData> camera_buffer;
    };
} // namespace caelus::core::components

#endif //CAELUS_SCENE_HPP
