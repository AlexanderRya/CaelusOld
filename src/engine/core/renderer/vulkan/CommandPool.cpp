#include "engine/core/renderer/vulkan/CommandPool.hpp"
#include "engine/core/details/Details.hpp"
#include "engine/logger/Logger.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::engine::core::vulkan {
    vk::CommandPool make_command_pool(const types::detail::DeviceDetails& details) {
        vk::CommandPoolCreateInfo command_pool_create_info{}; {
            command_pool_create_info.queueFamilyIndex = details.queue_family;
            command_pool_create_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        }

        auto pool = details.device.createCommandPool(command_pool_create_info);

        logger::info("Command pool successfully created");

        return pool;
    }
} // namespace caelus::engine::core::vulkan