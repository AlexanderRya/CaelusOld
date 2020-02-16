#ifndef CAELUS_COMMANDPOOL_HPP
#define CAELUS_COMMANDPOOL_HPP

namespace vk {
    class CommandPool;
} // namespace vk

namespace caelus::engine::core::types::detail {
    struct DeviceDetails;
} // namespace caelus::engine::core::types::detail

namespace caelus::engine::core::vulkan {
    vk::CommandPool make_command_pool(const types::detail::DeviceDetails&);
} // namespace caelus::engine::core::vulkan

#endif //CAELUS_COMMANDPOOL_HPP
