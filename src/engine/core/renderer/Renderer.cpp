#include "engine/core/Window.hpp"
#include "engine/core/Constants.hpp"
#include "engine/core/renderer/Renderer.hpp"
#include "engine/core/renderer/vulkan/Fence.hpp"
#include "engine/core/renderer/vulkan/Device.hpp"
#include "engine/core/info/PipelineCreateInfo.hpp"
#include "engine/core/renderer/vulkan/Instance.hpp"
#include "engine/core/renderer/vulkan/Swapchain.hpp"
#include "engine/core/renderer/vulkan/Semaphore.hpp"
#include "engine/core/renderer/vulkan/RenderPass.hpp"
#include "engine/core/renderer/vulkan/Framebuffer.hpp"
#include "engine/core/renderer/vulkan/CommandPool.hpp"
#include "engine/core/renderer/vulkan/CommandBuffer.hpp"
#include "engine/core/renderer/vulkan/DescriptorPool.hpp"
#include "engine/core/components/manager/ResourceManager.hpp"

namespace caelus::core {
    void Renderer::init(const Window& window) {
        vulkan_data.instance = vulkan::get_instance();
#if defined(CAELUS_DEBUG)
        vulkan_data.validation = vulkan::install_validation_layers(vulkan_data);
#endif
        vulkan_data.surface = window.create_surface(vulkan_data);
        vulkan_data.device_details = vulkan::get_device_details(vulkan_data);
        vulkan_data.swapchain_details = vulkan::get_swapchain_details(window, vulkan_data);
        vulkan_data.command_pool = vulkan::make_command_pool(vulkan_data);
        vulkan_data.transient_pool = vulkan::make_transient_pool(vulkan_data);
        vulkan_data.command_buffers = vulkan::make_command_buffer(vulkan_data);
        vulkan_data.descriptor_pool = vulkan::make_descriptor_pool(vulkan_data);
        vulkan_data.render_passes.emplace_back(vulkan::make_default_render_pass(vulkan_data));
        vulkan_data.framebuffers.emplace_back(vulkan::get_framebuffers(0, window, vulkan_data));
        // Synchronization
        vulkan_data.frames_in_flight.resize(constants::max_frames_in_flight);
        vulkan_data.image_available = vulkan::make_semaphores(constants::max_frames_in_flight, vulkan_data);
        vulkan_data.render_finished = vulkan::make_semaphores(constants::max_frames_in_flight, vulkan_data);
    }

    void Renderer::draw(const manager::ResourceManager& resource_manager) {
        auto res = vulkan_data.device_details.device.acquireNextImageKHR(
            vulkan_data.swapchain_details.swapchain,
            -1,
            vulkan_data.image_available[current_frame],
            nullptr,
            &image_index);

        if (res == vk::Result::eErrorOutOfDateKHR) {
            Window::resized = true;
        }

        if (!vulkan_data.frames_in_flight[current_frame]) {
            vulkan_data.frames_in_flight[current_frame] = vulkan::make_fence(vulkan_data);
        }

        vulkan_data.device_details.device.waitForFences(vulkan_data.frames_in_flight[current_frame], true, -1);

        record_buffers(resource_manager);

        std::array<vk::PipelineStageFlags, 1> wait_stages{ vk::PipelineStageFlagBits::eColorAttachmentOutput };

        vk::SubmitInfo submit_info{}; {
            submit_info.pWaitDstStageMask = wait_stages.data();
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &vulkan_data.command_buffers[image_index];
            submit_info.waitSemaphoreCount = 1;
            submit_info.pWaitSemaphores = &vulkan_data.image_available[current_frame];
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = &vulkan_data.render_finished[current_frame];
        }

        vulkan_data.device_details.device.resetFences(vulkan_data.frames_in_flight[current_frame]);
        vulkan_data.device_details.queue.submit(submit_info, vulkan_data.frames_in_flight[current_frame]);

        vk::PresentInfoKHR present_info{}; {
            present_info.swapchainCount = 1;
            present_info.pSwapchains = &vulkan_data.swapchain_details.swapchain;
            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores = &vulkan_data.render_finished[current_frame];
            present_info.pImageIndices = &image_index;
        }

        vulkan_data.device_details.queue.presentKHR(present_info);

        current_frame = (current_frame + 1) % constants::max_frames_in_flight;
    }

    void Renderer::record_buffers(const manager::ResourceManager& resource_manager) {
        vk::CommandBufferBeginInfo begin_info{}; {
            begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        }

        vulkan_data.command_buffers[image_index].begin(begin_info);

        vk::ClearValue clear_value{}; {
            clear_value.color = std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 0.0f };
        }

        vk::RenderPassBeginInfo render_pass_begin_info{}; {
            render_pass_begin_info.renderPass = vulkan_data.render_passes[0];
            render_pass_begin_info.framebuffer = vulkan_data.framebuffers[0][image_index];
            render_pass_begin_info.pClearValues = &clear_value;
            render_pass_begin_info.clearValueCount = 1;
            render_pass_begin_info.renderArea.extent = vulkan_data.swapchain_details.extent;
        }
        
        vulkan_data.command_buffers[image_index].beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

        vk::Viewport viewport{}; {
            viewport.x = 0;
            viewport.y = 0;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            viewport.width = vulkan_data.swapchain_details.extent.width;
            viewport.height = vulkan_data.swapchain_details.extent.height;
        }

        vk::Rect2D scissor{}; {
            scissor.extent = vulkan_data.swapchain_details.extent;
            scissor.offset = { { 0, 0 } };
        }

        vulkan_data.command_buffers[image_index].setViewport(0, viewport);
        vulkan_data.command_buffers[image_index].setScissor(0, scissor);

        for (const auto& mesh : resource_manager.get_meshes()) {
            vulkan_data.command_buffers[image_index].bindPipeline(
                vk::PipelineBindPoint::eGraphics,
                resource_manager.get_pipeline(mesh.pipeline_id).pipeline);

            vk::DeviceSize offset = 0;
            vulkan_data.command_buffers[image_index].bindVertexBuffers(0, mesh.vertex_buffer.buffer, offset);
            vulkan_data.command_buffers[image_index].bindVertexBuffers(1, mesh.instance_buffer.buffer, offset);
            vulkan_data.command_buffers[image_index].draw(mesh.vertices.size(), mesh.instances.size(), 0, 0);
        }

        vulkan_data.command_buffers[image_index].endRenderPass();
        vulkan_data.command_buffers[image_index].end();
    }
} // namespace caelus::core