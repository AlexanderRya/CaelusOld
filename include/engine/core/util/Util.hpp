#ifndef CAELUS_UTIL_HPP
#define CAELUS_UTIL_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::components::buffers {
    struct VertexBuffer;
} // namespace caelus::core::components::buffers

namespace caelus::core::util {
    [[nodiscard]] vk::Buffer make_buffer(const usize, const vk::BufferUsageFlags&, const types::detail::VulkanContext&);
    [[nodiscard]] vk::DeviceMemory allocate_memory(const vk::Buffer&, const vk::MemoryPropertyFlags&, const types::detail::VulkanContext&);
    void copy_buffer(const vk::Buffer&, const vk::Buffer&, const usize, const types::detail::VulkanContext&);
    [[nodiscard]] std::vector<types::Vertex> generate_triangle_geometry();
} // namespace caelus::core::util

#endif //CAELUS_UTIL_HPP
