#ifndef CAELUS_DESCRIPTORSET_HPP
#define CAELUS_DESCRIPTORSET_HPP

#include "engine/core/Types.hpp"

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace vk {
    class DescriptorSet;
} // namespace vk

namespace caelus::core::vulkan {
    class DescriptorSet {
        const types::detail::VulkanContext* ctx;
    public:
        std::vector<vk::DescriptorSet> handles{};

        DescriptorSet() = default;

        void create(const types::detail::VulkanContext*, const vk::DescriptorSetLayout&);
        void bind();

        vk::DescriptorSet& operator [](const usize);
    };
} // namespace caelus::core::vulkan

#endif //CAELUS_DESCRIPTORSET_HPP
