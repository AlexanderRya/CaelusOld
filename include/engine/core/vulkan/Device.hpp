#ifndef CAELUS_DEVICE_HPP
#define CAELUS_DEVICE_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::types::detail {
    struct DeviceDetails;
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    [[nodiscard]] types::detail::DeviceDetails get_device_details(const types::detail::VulkanContext&);
    [[nodiscard]] u32 find_memory_type(const types::detail::VulkanContext&, const u32, const vk::MemoryPropertyFlags&);

} // namespace caelus::core::vulkan

#endif //CAELUS_DEVICE_HPP
