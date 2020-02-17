#include "engine/core/renderer/vulkan/CommandBuffer.hpp"
#include "engine/core/details/VulkanData.hpp"
#include "engine/logger/Logger.hpp"
#include "vulkan/vulkan.hpp"
#include <vector>

namespace caelus::core::vulkan {
    std::vector<vk::CommandBuffer> make_command_buffer(const types::detail::VulkanData& data) {
        vk::CommandBufferAllocateInfo allocate_info{}; {
            allocate_info.commandPool = data.command_pool;
            allocate_info.commandBufferCount = data.swapchain_details.image_count;
            allocate_info.level = vk::CommandBufferLevel::ePrimary;
        }

        auto buffers = data.device_details.device.allocateCommandBuffers(allocate_info);

        logger::info("Created ", allocate_info.commandBufferCount, " command buffers for rendering");

        return buffers;
    }
} // namespace caelus::core::vulkan