#ifndef CAELUS_INSTANCEBUFFER_HPP
#define CAELUS_INSTANCEBUFFER_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"
#include <vector>

namespace caelus::core::types::detail {
    struct VulkanData;
} // namespace caelus::core::types::detail

namespace caelus::core::components::buffers {
    struct InstanceBuffer {
        vk::Buffer buffer;
        vk::DeviceMemory memory;

        InstanceBuffer() = default;

        void allocate(const std::vector<glm::mat4>&, const types::detail::VulkanData&);
    };
} // namespace caelus::core::components::buffers

#endif //CAELUS_INSTANCEBUFFER_HPP
