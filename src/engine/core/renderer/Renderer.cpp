#include <engine/core/info/PipelineCreateInfo.hpp>
#include "engine/core/renderer/Renderer.hpp"
#include "engine/core/renderer/vulkan/Device.hpp"
#include "engine/core/details/ApplicationData.hpp"
#include "engine/core/renderer/vulkan/Instance.hpp"
#include "engine/core/renderer/vulkan/Swapchain.hpp"
#include "engine/core/renderer/vulkan/RenderPass.hpp"
#include "engine/core/renderer/vulkan/Framebuffer.hpp"
#include "engine/core/renderer/vulkan/CommandPool.hpp"
#include "engine/core/renderer/vulkan/DescriptorPool.hpp"

namespace caelus::core {
    void Renderer::init(const types::detail::ApplicationData& application_data) {
        vulkan_data.instance = vulkan::get_instance();
#if defined(CAELUS_DEBUG)
        vulkan_data.validation = vulkan::install_validation_layers(vulkan_data);
#endif
        vulkan_data.surface = application_data.window.create_surface(vulkan_data);
        vulkan_data.device_details = vulkan::get_device_details(vulkan_data);
        vulkan_data.swapchain_details = vulkan::get_swapchain_details(application_data.window, vulkan_data);
        vulkan_data.command_pool = vulkan::make_command_pool(vulkan_data);
        vulkan_data.descriptor_pool = vulkan::make_descriptor_pool(vulkan_data);
        vulkan_data.render_passes.emplace_back(vulkan::make_default_render_pass(vulkan_data));
        vulkan_data.framebuffers = vulkan::get_framebuffers(application_data.window, vulkan_data);
    }
} // namespace caelus::core