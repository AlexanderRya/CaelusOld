#ifndef CAELUS_DESCRIPTORSETCREATEINFO_HPP
#define CAELUS_DESCRIPTORSETCREATEINFO_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::types::info {
    struct DescriptorSetCreateInfo {
        const types::detail::VulkanContext* ctx{};
        vk::DescriptorType descriptor_type{};
        vk::BufferUsageFlags usage_flags{};
        u32 binding{};
        u32 layout_id{};
    };
} // namespace caelus::core::types::info

#endif //CAELUS_DESCRIPTORSETCREATEINFO_HPP
