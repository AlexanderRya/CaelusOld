#ifndef CAELUS_INSTANCE_HPP
#define CAELUS_INSTANCE_HPP

#include "engine/core/Types.hpp"

namespace vk {
    class Instance;
    class DebugUtilsMessengerEXT;
} // namespace vk

namespace caelus::core::types::detail {
    struct VulkanData;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    [[nodiscard]] vk::Instance get_instance();
    [[nodiscard]] vk::DebugUtilsMessengerEXT install_validation_layers(const types::detail::VulkanData&);
} // namespace caelus::core::vulkan

#endif //CAELUS_INSTANCE_HPP
