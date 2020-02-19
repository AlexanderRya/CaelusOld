#ifndef CAELUS_VERTEXBUFFER_HPP
#define CAELUS_VERTEXBUFFER_HPP

#include "vulkan/vulkan.hpp"
#include <vector>

namespace caelus::core::types::detail {
    struct VulkanData;
} // namespace caelus::core::types::detail

namespace caelus::core::types {
    struct Vertex;
} // namespace caelus::core::types

namespace caelus::core::components::buffers {
    struct VertexBuffer {
        vk::Buffer buffer;
        vk::DeviceMemory memory;

        VertexBuffer() = default;

        void allocate(const std::vector<types::Vertex>&, const types::detail::VulkanData&);
    };
} // namespace caelus::core::components::buffers

#endif //CAELUS_VERTEXBUFFER_HPP
