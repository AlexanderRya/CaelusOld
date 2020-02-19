#include "engine/core/renderer/vulkan/Framebuffer.hpp"
#include "engine/core/details/VulkanContext.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Window.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::vulkan {
    std::vector<vk::Framebuffer> get_framebuffers(const u32 id, const Window& window, const types::detail::VulkanContext& ctx) {
        std::vector<vk::Framebuffer> framebuffers{};
        framebuffers.reserve(ctx.swapchain_details.image_count);

        vk::FramebufferCreateInfo framebuffer_create_info{}; {
            framebuffer_create_info.attachmentCount = ctx.swapchain_details.image_count;
            framebuffer_create_info.renderPass = ctx.render_passes[id];
            framebuffer_create_info.height = window.height;
            framebuffer_create_info.width = window.width;
            framebuffer_create_info.attachmentCount = 1;
            framebuffer_create_info.layers = 1;
        }

        for (const auto& image_view : ctx.swapchain_details.image_views) {
            framebuffer_create_info.pAttachments = &image_view;
            framebuffers.emplace_back(ctx.device_details.device.createFramebuffer(framebuffer_create_info));
        }

        logger::info(
            "Framebuffers successfully created with renderpass at index: ", id);

        return framebuffers;
    }
} // namespace caelus::core::vulkan