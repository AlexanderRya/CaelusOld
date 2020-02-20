#ifndef CAELUS_DESCRIPTORBUFFER_HPP
#define CAELUS_DESCRIPTORBUFFER_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"
#include <vector>

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::components::buffers {
    struct DescriptorBuffer {
        vk::Buffer buffer;
        vk::DeviceMemory memory;

        usize current_size{};
        vk::BufferUsageFlags usage_flags{};

        void* mapped{};

        DescriptorBuffer() = default;

        void allocate(const usize, const vk::BufferUsageFlags& flags, const types::detail::VulkanContext&);
        void write(const void*, const usize, const types::detail::VulkanContext&);
        void deallocate(const types::detail::VulkanContext&);
    };
} // namespace caelus::core::components::buffers

#endif //CAELUS_DESCRIPTORBUFFER_HPP
