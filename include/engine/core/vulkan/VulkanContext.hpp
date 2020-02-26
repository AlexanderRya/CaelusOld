#ifndef CAELUS_CONTEXT_HPP
#define CAELUS_CONTEXT_HPP

#include "engine/core/details/Details.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::types::detail {
    struct VulkanContext {
        vk::DispatchLoaderDynamic dispatcher;
        // Main vulkan data
        vk::Instance instance;
        vk::DebugUtilsMessengerEXT validation;
        vk::SurfaceKHR surface;
        DeviceDetails device_details;
        SwapchainDetails swapchain_details;
        vk::CommandPool command_pool;
        vk::CommandPool transient_pool;
        std::vector<vk::CommandBuffer> command_buffers;
        vk::DescriptorPool descriptor_pool;
        std::vector<vk::RenderPass> render_passes;
        std::vector<std::vector<vk::Framebuffer>> framebuffers;
        std::vector<vk::Semaphore> image_available;
        std::vector<vk::Semaphore> render_finished;
    };
} // namespace caelus::core::types::detail

namespace caelus::core {
    class Window;
} // namespace caelus::core

namespace caelus::core::vulkan {
    [[nodiscard]] types::detail::VulkanContext make_vulkan_context(const Window&);
} // namespace caelus::core::vulkan

#endif //CAELUS_VULKANCONTEXT_HPP
