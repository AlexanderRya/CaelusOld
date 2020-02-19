#ifndef CAELUS_COMMANDBUFFER_HPP
#define CAELUS_COMMANDBUFFER_HPP

#include <vector>

namespace vk {
    class CommandBuffer;
} // namespace vk

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    std::vector<vk::CommandBuffer> make_command_buffer(const types::detail::VulkanContext&);
} // namespace caelus::core::vulkan

#endif //CAELUS_COMMANDBUFFER_HPP
