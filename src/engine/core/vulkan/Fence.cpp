#include "engine/core/vulkan/Fence.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::vulkan {
    [[nodiscard]] std::vector<vk::Fence> make_fences(const u32 size, const types::detail::VulkanContext& ctx) {
        vk::FenceCreateInfo fence_create_info{}; {
            fence_create_info.flags = vk::FenceCreateFlagBits::eSignaled;
        }

        std::vector<vk::Fence> fences(size, ctx.device_details.device.createFence(fence_create_info, nullptr, ctx.dispatcher));

        return fences;
    }

    vk::Fence make_fence(const types::detail::VulkanContext& ctx) {
        vk::FenceCreateInfo fence_create_info{}; {
            fence_create_info.flags = vk::FenceCreateFlagBits::eSignaled;
        }

        return ctx.device_details.device.createFence(fence_create_info, nullptr, ctx.dispatcher);
    }
} // namespace caelus::core::vulkan