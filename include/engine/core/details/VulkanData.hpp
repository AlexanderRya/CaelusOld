#ifndef CAELUS_VULKANDATA_HPP
#define CAELUS_VULKANDATA_HPP

#include "engine/core/details/Details.hpp"
#include "engine/core/Window.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::types::detail {
    struct VulkanData {
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
        std::vector<vk::Fence> frames_in_flight;
        std::vector<vk::Semaphore> image_available;
        std::vector<vk::Semaphore> render_finished;
    };
} // caelus::core::types::detail

#endif //CAELUS_VULKANDATA_HPP
