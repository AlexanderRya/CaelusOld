#ifndef CAELUS_VERTEXBUFFER_HPP
#define CAELUS_VERTEXBUFFER_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"
#include <vector>

namespace caelus::core::types {
    struct Vertex;
} // namespace caelus::core::types

namespace caelus::core::types {
    namespace detail {
        struct VulkanContext;
    } // namespace caelus::core::types::detail

    namespace info {
        struct VertexBufferCreateInfo {
            std::vector<types::Vertex> vertices;
            const types::detail::VulkanContext* ctx;
        };
    } // namespace caelus::core::types::info
} // namespace caelus::core::types

namespace caelus::core::components::buffers {
    struct VertexBuffer {
        vk::Buffer buffer;
        vk::DeviceMemory memory;

        explicit VertexBuffer(const types::info::VertexBufferCreateInfo&);

        void deallocate();
    private:
        const types::detail::VulkanContext* ctx;
    };
} // namespace caelus::core::components::buffers

#endif //CAELUS_VERTEXBUFFER_HPP
