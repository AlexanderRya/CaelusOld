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
#include "engine/core/Intrinsics.hpp"
#include "engine/core/Constants.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Window.hpp"

namespace caelus::core::vulkan {
    static inline void load_initial_modules(vk::DispatchLoaderDynamic& dispatcher) {
        auto module = util::load_module(vulkan_module);

        dispatcher.vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(util::load_symbol(module, "vkCreateInstance"));
        dispatcher.vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(util::load_symbol(module, "vkEnumerateInstanceExtensionProperties"));
        dispatcher.vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(util::load_symbol(module, "vkGetInstanceProcAddr"));

        util::close_module(module);
    }

    [[nodiscard]] types::detail::VulkanContext make_vulkan_context(const Window& window) {
        types::detail::VulkanContext context{};

        logger::info("Vulkan initialization sequence starting.");
        load_initial_modules(context.dispatcher);
        context.instance = vulkan::get_instance(context);
        context.dispatcher.init(context.instance, context.dispatcher.vkGetInstanceProcAddr);
#if defined(CAELUS_DEBUG)
        logger::warning("Vulkan debug mode active, performance may be lower than usual");
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
        context.framebuffers.emplace_back(vulkan::get_framebuffers(0, context));
        // Synchronization
        context.image_available = vulkan::make_semaphores(constants::frames_in_flight, context);
        context.render_finished = vulkan::make_semaphores(constants::frames_in_flight, context);
        logger::info("Vulkan initialization sequence completed successfully.");

        return context;
    }
} // namespace caelus::core::vulkan