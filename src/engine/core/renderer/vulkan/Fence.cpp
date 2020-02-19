#include "engine/core/renderer/vulkan/Fence.hpp"
#include "engine/core/details/VulkanContext.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::vulkan {
    [[nodiscard]] std::vector<vk::Fence> make_fences(const u32 size, const types::detail::VulkanContext& ctx) {
        vk::FenceCreateInfo fence_create_info{}; {
            fence_create_info.flags = vk::FenceCreateFlagBits::eSignaled;
        }

        std::vector<vk::Fence> fences(size, ctx.device_details.device.createFence(fence_create_info));

        return fences;
    }

    vk::Fence make_fence(const types::detail::VulkanContext& data) {
        vk::FenceCreateInfo fence_create_info{}; {
            fence_create_info.flags = vk::FenceCreateFlagBits::eSignaled;
        }

        return data.device_details.device.createFence(fence_create_info);
    }
} // namespace caelus::core::vulkan