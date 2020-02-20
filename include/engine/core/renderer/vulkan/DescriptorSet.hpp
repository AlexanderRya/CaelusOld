#ifndef CAELUS_DESCRIPTORSET_HPP
#define CAELUS_DESCRIPTORSET_HPP

#include "engine/core/details/Details.hpp"
#include <vector>

namespace caelus::core::types::info {
    struct DescriptorSetInfo;
} // namespace caelus::core::types::info

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    struct DescriptorSet {
        types::detail::DescriptorSetDetails details;

        void create(const types::info::DescriptorSetInfo&);
        void bind_buffer(const u32, const types::detail::VulkanContext&);
        void update(const types::detail::VulkanContext&);
    };

    [[nodiscard]] std::vector<DescriptorSet> make_descriptor_sets(const types::info::DescriptorSetInfo&);
}

#endif //CAELUS_DESCRIPTORSET_HPP
