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
#include "engine/core/components/manager/DescriptorSetManager.hpp"
#include "engine/core/components/manager/PipelineLayoutManager.hpp"

namespace caelus::core {
    void Renderer::init(const Window& window) {
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
    }

    void Renderer::draw() {
        auto res = context.device_details.device.acquireNextImageKHR(
            context.swapchain_details.swapchain,
            -1,
            context.image_available[current_frame],
            nullptr,
            &image_index);

        if (res == vk::Result::eErrorOutOfDateKHR) {
            Window::resized = true;
        }

        if (!context.frames_in_flight[current_frame]) {
            context.frames_in_flight[current_frame] = vulkan::make_fence(context);
        }

        context.device_details.device.waitForFences(context.frames_in_flight[current_frame], true, -1);

        record_buffers();

        std::array<vk::PipelineStageFlags, 1> wait_stages{ vk::PipelineStageFlagBits::eColorAttachmentOutput };

        vk::SubmitInfo submit_info{}; {
            submit_info.pWaitDstStageMask = wait_stages.data();
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &context.command_buffers[image_index];
            submit_info.waitSemaphoreCount = 1;
            submit_info.pWaitSemaphores = &context.image_available[current_frame];
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = &context.render_finished[current_frame];
        }

        context.device_details.device.resetFences(context.frames_in_flight[current_frame]);
        context.device_details.queue.submit(submit_info, context.frames_in_flight[current_frame]);

        vk::PresentInfoKHR present_info{}; {
            present_info.swapchainCount = 1;
            present_info.pSwapchains = &context.swapchain_details.swapchain;
            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores = &context.render_finished[current_frame];
            present_info.pImageIndices = &image_index;
        }

        context.device_details.queue.presentKHR(present_info);

        current_frame = (current_frame + 1) % constants::max_frames_in_flight;
    }

    void Renderer::record_buffers() {
        vk::CommandBufferBeginInfo begin_info{}; {
            begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        }

        context.command_buffers[image_index].begin(begin_info);

        vk::ClearValue clear_value{}; {
            clear_value.color = vk::ClearColorValue{ std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 0.0f } };
        }

        vk::RenderPassBeginInfo render_pass_begin_info{}; {
            render_pass_begin_info.renderPass = context.render_passes[0];
            render_pass_begin_info.framebuffer = context.framebuffers[0][image_index];
            render_pass_begin_info.pClearValues = &clear_value;
            render_pass_begin_info.clearValueCount = 1;
            render_pass_begin_info.renderArea.extent = context.swapchain_details.extent;
        }
        
        context.command_buffers[image_index].beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

        vk::Viewport viewport{}; {
            viewport.x = 0;
            viewport.y = 0;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            viewport.width = context.swapchain_details.extent.width;
            viewport.height = context.swapchain_details.extent.height;
        }

        vk::Rect2D scissor{}; {
            scissor.extent = context.swapchain_details.extent;
            scissor.offset = { { 0, 0 } };
        }

        context.command_buffers[image_index].setViewport(0, viewport);
        context.command_buffers[image_index].setScissor(0, scissor);

        for (const auto& mesh : manager::ResourceManager::get_meshes()) {
            update_sets(0);

            context.command_buffers[image_index].bindPipeline(
                vk::PipelineBindPoint::eGraphics,
                manager::ResourceManager::get_pipeline(mesh.pipeline_id).pipeline);

            context.command_buffers[image_index].bindVertexBuffers(0, mesh.vertex_buffer.buffer, 0ul);
            context.command_buffers[image_index].bindVertexBuffers(1, mesh.instance_buffer.buffer, 0ul);
            context.command_buffers[image_index].bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                manager::PipelineLayoutManager::get_layout(mesh.pipeline_layout_id),
                0,
                manager::DescriptorSetManager::get_descriptor_sets<types::TransformUBO>(mesh.descriptor_set_id)[current_frame].descriptor_set,
                nullptr);
            context.command_buffers[image_index].draw(mesh.vertices.size(), mesh.instances.size(), 0, 0);
        }

        context.command_buffers[image_index].endRenderPass();
        context.command_buffers[image_index].end();
    }

    void Renderer::update_sets(const u32 set_id) {
        static types::TransformUBO ubo{}; {
            ubo.model = glm::mat4(1.0f);
            ubo.pv_mat = glm::mat4(1.0f);
        }

        for (auto& each : manager::DescriptorSetManager::get_descriptor_sets<types::TransformUBO>(set_id)) {
            each.write({ ubo }, context);
        }
    }
} // namespace caelus::core