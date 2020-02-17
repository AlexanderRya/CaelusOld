#include "engine/core/renderer/vulkan/Semaphore.hpp"
#include "engine/core/details/VulkanData.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::vulkan {
    [[nodiscard]] std::vector<vk::Semaphore> make_semaphores(const u32 size, const types::detail::VulkanData& data) {
        vk::SemaphoreCreateInfo semaphore_create_info{};

        std::vector<vk::Semaphore> semaphores(size, data.device_details.device.createSemaphore(semaphore_create_info));

        return semaphores;
    }
} // namespace caelus::core::vulkan