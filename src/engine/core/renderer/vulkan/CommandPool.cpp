#include "engine/core/renderer/vulkan/CommandPool.hpp"
#include "engine/core/details/VulkanData.hpp"
#include "engine/logger/Logger.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::vulkan {
    vk::CommandPool make_command_pool(const types::detail::VulkanData& data) {
        vk::CommandPoolCreateInfo command_pool_create_info{}; {
            command_pool_create_info.queueFamilyIndex = data.device_details.queue_family;
            command_pool_create_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        }

        auto pool = data.device_details.device.createCommandPool(command_pool_create_info);

        logger::info("Command pool successfully created");

        return pool;
    }

    vk::CommandPool make_transient_pool(const types::detail::VulkanData& data) {
        vk::CommandPoolCreateInfo command_pool_create_info{}; {
            command_pool_create_info.queueFamilyIndex = data.device_details.queue_family;
            command_pool_create_info.flags =
                vk::CommandPoolCreateFlagBits::eResetCommandBuffer |
                vk::CommandPoolCreateFlagBits::eTransient;
        }

        auto pool = data.device_details.device.createCommandPool(command_pool_create_info);

        logger::info("Transient command pool successfully created");

        return pool;
    }
} // namespace caelus::core::vulkan