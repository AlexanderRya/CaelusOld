#include "engine/core/vulkan/Semaphore.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::vulkan {
    [[nodiscard]] std::vector<vk::Semaphore> make_semaphores(const u32 size, const types::detail::VulkanContext& ctx) {
        vk::SemaphoreCreateInfo semaphore_create_info{};

        std::vector<vk::Semaphore> semaphores(size, ctx.device_details.device.createSemaphore(semaphore_create_info));

        return semaphores;
    }
} // namespace caelus::core::vulkan