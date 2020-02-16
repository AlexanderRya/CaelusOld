#ifndef CAELUS_DESCRIPTORPOOL_HPP
#define CAELUS_DESCRIPTORPOOL_HPP

namespace vk {
    class DescriptorPool;
} // namespace vk

namespace caelus::engine::core::types::detail {
    struct VulkanData;
} // namespace caelus::engine::core::types::detail

namespace caelus::engine::core::vulkan {
    [[nodiscard]] vk::DescriptorPool make_descriptor_pool(const types::detail::VulkanData&);
} // namespace caelus::engine::core::vulkan

#endif //CAELUS_DESCRIPTORPOOL_HPP
