#ifndef CAELUS_COMMANDPOOL_HPP
#define CAELUS_COMMANDPOOL_HPP

namespace vk {
    class CommandPool;
} // namespace vk

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    vk::CommandPool make_command_pool(const types::detail::VulkanContext&);
    vk::CommandPool make_transient_pool(const types::detail::VulkanContext&);
} // namespace caelus::core::vulkan

#endif //CAELUS_COMMANDPOOL_HPP
