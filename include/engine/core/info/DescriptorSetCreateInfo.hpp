#ifndef CAELUS_DESCRIPTORSETCREATEINFO_HPP
#define CAELUS_DESCRIPTORSETCREATEINFO_HPP

#include "engine/experimental/BufferContainer.hpp"
#include "engine/core/details/VulkanContext.hpp"
#include "engine/core/Types.hpp"

namespace caelus::core::types::info {
    struct DescriptorSetCreateInfo {
        u32 binding;

        experimental::components::buffers::BufferType::ActiveType type;
        u32 layout_id;
        const types::detail::VulkanContext* ctx;
    };
} // namespace caelus::core::types::info

#endif //CAELUS_DESCRIPTORSETCREATEINFO_HPP
