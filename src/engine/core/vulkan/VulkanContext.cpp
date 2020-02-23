#include "engine/core/vulkan/DescriptorPool.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/vulkan/CommandBuffer.hpp"
#include "engine/core/vulkan/CommandPool.hpp"
#include "engine/core/vulkan/Framebuffer.hpp"
#include "engine/core/vulkan/RenderPass.hpp"
#include "engine/core/vulkan/Semaphore.hpp"
#include "engine/core/vulkan/Swapchain.hpp"
#include "engine/core/vulkan/Instance.hpp"
#include "engine/core/vulkan/Device.hpp"
#include "engine/core/Constants.hpp"
#include "engine/core/Window.hpp"

namespace caelus::core::vulkan {
    [[nodiscard]] types::detail::VulkanContext make_vulkan_context(const Window& window) {
        types::detail::VulkanContext context{};

        context.instance = vulkan::get_instance();
#if defined(CAELUS_DEBUG)
        context.validation = vulkan::install_validation_layers(context);
#endif
        context.surface = window.create_surface(context);
        context.device_details = vulkan::get_device_details(context);
        context.swapchain_details = vulkan::get_swapchain_details(window, context);
        context.command_pool = vulkan::make_command_pool(context);
        context.transient_pool = vulkan::make_transient_pool(context);
        context.command_buffers = vulkan::make_command_buffer(context);
        context.descriptor_pool = vulkan::make_descriptor_pool(context);
        context.render_passes.emplace_back(vulkan::make_default_render_pass(context));
        context.framebuffers.emplace_back(vulkan::get_framebuffers(0, window, context));
        // Synchronization
        context.frames_in_flight.resize(constants::max_frames_in_flight);
        context.image_available = vulkan::make_semaphores(constants::max_frames_in_flight, context);
        context.render_finished = vulkan::make_semaphores(constants::max_frames_in_flight, context);

        return context;
    }
} // namespace caelus::core::vulkan