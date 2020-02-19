#ifndef CAELUS_DEVICE_HPP
#define CAELUS_DEVICE_HPP

#include "engine/core/Types.hpp"

namespace vk {
    class Instance;
    class SurfaceKHR;
} // namespace vk

namespace caelus::core::types::detail {
    struct DeviceDetails;
    struct VulkanData;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    [[nodiscard]] types::detail::DeviceDetails get_device_details(const types::detail::VulkanData&);
    [[nodiscard]] u32 find_memory_type(const types::detail::VulkanData&, const u32, const vk::MemoryPropertyFlags&);

} // namespace caelus::core::vulkan

#endif //CAELUS_DEVICE_HPP
