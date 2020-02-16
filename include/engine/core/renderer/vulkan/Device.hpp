#ifndef CAELUS_DEVICE_HPP
#define CAELUS_DEVICE_HPP

#include "engine/Types.hpp"

namespace vk {
    class Instance;
    class SurfaceKHR;
} // namespace vk

namespace caelus::engine::core::types::detail {
    struct DeviceDetails;
    struct VulkanData;
} // namespace caelus::engine::core::types::detail

namespace caelus::engine::core::vulkan {
    [[nodiscard]] types::detail::DeviceDetails get_device_details(const types::detail::VulkanData&);
} // namespace caelus::engine::core::vulkan

#endif //CAELUS_DEVICE_HPP
