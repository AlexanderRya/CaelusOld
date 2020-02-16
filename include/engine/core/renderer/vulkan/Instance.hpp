#ifndef CAELUS_INSTANCE_HPP
#define CAELUS_INSTANCE_HPP

#include "engine/Types.hpp"

namespace vk {
    class Instance;
    class DebugUtilsMessengerEXT;
} // namespace vk

namespace caelus::engine::core::types::detail {
    struct VulkanData;
} // namespace caelus::engine::core::types::detail

namespace caelus::engine::core::vulkan {
    [[nodiscard]] vk::Instance get_instance();
    [[nodiscard]] vk::DebugUtilsMessengerEXT install_validation_layers(const types::detail::VulkanData&);
} // namespace caelus::engine::core::vulkan

#endif //CAELUS_INSTANCE_HPP
