#ifndef CAELUS_UNIFORMBUFFER_HPP
#define CAELUS_UNIFORMBUFFER_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::components::buffers {
    struct UniformBuffer {
        vk::Buffer buffer{};
        vk::DeviceMemory memory{};

        // Persistently mapped
        void* mapped{};

        UniformBuffer() = default;

        void allocate(const types::detail::VulkanContext&);
        void write(const types::TransformUBO&);
        void deallocate(const types::detail::VulkanContext&);
    };
} // namespace caelus::core::components::buffers

#endif //CAELUS_UNIFORMBUFFER_HPP
