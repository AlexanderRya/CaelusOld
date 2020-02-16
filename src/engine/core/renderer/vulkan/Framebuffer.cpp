#include "engine/core/renderer/vulkan/Framebuffer.hpp"
#include "engine/core/details/VulkanData.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Window.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::engine::core::vulkan {
    std::vector<vk::Framebuffer> get_framebuffers(const Window& window, const types::detail::VulkanData& data) {
        std::vector<vk::Framebuffer> framebuffers{};
        framebuffers.reserve(data.swapchain_details.image_count);

        vk::FramebufferCreateInfo framebuffer_create_info{}; {
            framebuffer_create_info.attachmentCount = data.swapchain_details.image_count;
            framebuffer_create_info.renderPass = data.render_passes[0];
            framebuffer_create_info.height = window.height;
            framebuffer_create_info.width = window.width;
            framebuffer_create_info.attachmentCount = 1;
            framebuffer_create_info.layers = 1;
        }

        for (const auto& image_view : data.swapchain_details.image_views) {
            framebuffer_create_info.pAttachments = &image_view;
            framebuffers.emplace_back(data.device_details.device.createFramebuffer(framebuffer_create_info));
        }

        logger::info(
            "Framebuffers successfully created with renderpass, index: ", 0);

        return framebuffers;
    }
} // namespace caelus::engine::core::vulkan