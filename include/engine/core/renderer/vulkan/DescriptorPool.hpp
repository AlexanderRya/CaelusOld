#ifndef CAELUS_DESCRIPTORPOOL_HPP
#define CAELUS_DESCRIPTORPOOL_HPP

namespace vk {
    class DescriptorPool;
} // namespace vk

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    [[nodiscard]] vk::DescriptorPool make_descriptor_pool(const types::detail::VulkanContext&);
} // namespace caelus::core::vulkan

#endif //CAELUS_DESCRIPTORPOOL_HPP
