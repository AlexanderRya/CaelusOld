#include "engine/core/vulkan/CommandBuffer.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/logger/Logger.hpp"
#include "vulkan/vulkan.hpp"
#include <vector>

namespace caelus::core::vulkan {
    std::vector<vk::CommandBuffer> make_command_buffer(const types::detail::VulkanContext& ctx) {
        vk::CommandBufferAllocateInfo allocate_info{}; {
            allocate_info.commandPool = ctx.command_pool;
            allocate_info.commandBufferCount = ctx.swapchain_details.image_count;
            allocate_info.level = vk::CommandBufferLevel::ePrimary;
        }

        auto buffers = ctx.device_details.device.allocateCommandBuffers(allocate_info, ctx.dispatcher);

        logger::info("Created ", allocate_info.commandBufferCount, " command buffers for rendering");

        return buffers;
    }
} // namespace caelus::core::vulkan