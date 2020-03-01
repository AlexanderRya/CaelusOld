#ifndef CAELUS_MESH_HPP
#define CAELUS_MESH_HPP

#include "engine/core/components/buffers/GenericBuffer.hpp"
#include "engine/core/vulkan/DescriptorSet.hpp"
#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"
#include <vector>

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    struct Pipeline;
} // namespace caelus::core::vulkan

namespace caelus::core::components {
    struct Mesh {
        usize vertex_buffer_idx{};

        std::vector<glm::mat4> instances{};
        vulkan::DescriptorSet instance_descriptor{};
        buffers::GenericBuffer<glm::mat4> instance_buffer{};

        void (*update)(Mesh*) = nullptr;

        //!TODO: Textures.
    };

    void create_mesh(Mesh&, const vulkan::Pipeline&, const types::detail::VulkanContext&);
} // namespace caelus::core::components

#endif //CAELUS_MESH_HPP
