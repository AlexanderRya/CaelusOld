#include "engine/core/components/buffers/GenericBuffer.hpp"
#include "engine/core/components/buffers/VertexBuffer.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/vulkan/DescriptorSet.hpp"
#include "engine/core/renderer/Renderer.hpp"
#include "engine/core/components/Scene.hpp"
#include "engine/core/vulkan/Pipeline.hpp"
#include "engine/core/components/Mesh.hpp"
#include "engine/core/vulkan/Fence.hpp"
#include "engine/core/Constants.hpp"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace caelus::core {
    Renderer::Renderer(const types::detail::VulkanContext& ctx)
        : ctx(ctx) {
        frames_in_flight.resize(constants::frames_in_flight);
    }

    u32 Renderer::acquire_frame() {
        image_index = ctx.device_details.device.acquireNextImageKHR(ctx.swapchain_details.swapchain, -1, ctx.image_available[current_frame], nullptr, ctx.dispatcher).value;

        if (!frames_in_flight[current_frame]) {
            frames_in_flight[current_frame] = vulkan::make_fence(ctx);
        }

        ctx.device_details.device.waitForFences(frames_in_flight[current_frame], true, -1, ctx.dispatcher);

        return current_frame;
    }

    void Renderer::draw() {
        vk::PipelineStageFlags wait_mask{ vk::PipelineStageFlagBits::eColorAttachmentOutput };
        vk::SubmitInfo submit_info{}; {
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &ctx.command_buffers[image_index];
            submit_info.pWaitDstStageMask = &wait_mask;
            submit_info.waitSemaphoreCount = 1;
            submit_info.pWaitSemaphores = &ctx.image_available[current_frame];
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = &ctx.render_finished[current_frame];
        }

        ctx.device_details.device.resetFences(frames_in_flight[current_frame], ctx.dispatcher);
        ctx.device_details.queue.submit(submit_info, frames_in_flight[current_frame], ctx.dispatcher);

        vk::PresentInfoKHR present_info{}; {
            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores = &ctx.render_finished[current_frame];
            present_info.swapchainCount = 1;
            present_info.pSwapchains = &ctx.swapchain_details.swapchain;
            present_info.pImageIndices = &image_index;
        }

        ctx.device_details.queue.presentKHR(present_info, ctx.dispatcher);

        current_frame = (current_frame + 1) % constants::frames_in_flight;
    }

    void Renderer::build(components::Scene& scene) {
        vk::CommandBufferBeginInfo begin_info{}; {
            begin_info.flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        }

        ctx.command_buffers[image_index].begin(begin_info, ctx.dispatcher);

        vk::ClearValue clear_value{}; {
            clear_value.color = vk::ClearColorValue{ std::array{ 0.0f, 0.0f, 0.0f, 0.0f } };
        }

        vk::RenderPassBeginInfo render_pass_begin_info{}; {
            render_pass_begin_info.renderArea.extent = ctx.swapchain_details.extent;
            render_pass_begin_info.framebuffer = ctx.framebuffers[0][image_index];
            render_pass_begin_info.renderPass = ctx.render_passes[0];
            render_pass_begin_info.clearValueCount = 1;
            render_pass_begin_info.pClearValues = &clear_value;
        }

        vk::Viewport viewport{}; {
            viewport.width = ctx.swapchain_details.extent.width;
            viewport.height = ctx.swapchain_details.extent.height;
            viewport.x = 0;
            viewport.y = 0;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
        }

        vk::Rect2D scissor{}; {
            scissor.extent = ctx.swapchain_details.extent;
            scissor.offset = { { 0, 0 } };
        }

        ctx.command_buffers[image_index].setViewport(0, viewport, ctx.dispatcher);
        ctx.command_buffers[image_index].setScissor(0, scissor, ctx.dispatcher);

        ctx.command_buffers[image_index].beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline, ctx.dispatcher);

        ctx.command_buffers[image_index].bindPipeline(vk::PipelineBindPoint::eGraphics, scene.mesh_pipeline.pipeline, ctx.dispatcher);
        //ctx.command_buffers[image_index].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, scene.mesh_pipeline.layout.pipeline_layout, 0, scene.camera_descriptor[current_frame].handle(), nullptr, ctx.dispatcher);

        for (auto& mesh : scene.meshes) {
            mesh.update(&mesh);
            update_mesh(mesh);

            ctx.command_buffers[image_index].bindVertexBuffers(0, scene.vertex_buffers[mesh.vertex_buffer_idx].buffer, 0ul, ctx.dispatcher);
            ctx.command_buffers[image_index].bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                scene.mesh_pipeline.layout.pipeline_layout,
                0,
                mesh.instance_descriptor[current_frame].handle(),
                nullptr, ctx.dispatcher);
            ctx.command_buffers[image_index].draw(scene.vertex_buffers[mesh.vertex_buffer_idx].size, mesh.instances.size(), 0, 0, ctx.dispatcher);
        }

        ctx.command_buffers[image_index].endRenderPass(ctx.dispatcher);
        ctx.command_buffers[image_index].end(ctx.dispatcher);
    }

    void Renderer::update_mesh(components::Mesh& mesh) {
        if (mesh.instances.size() != mesh.instance_buffer[current_frame].size()) {
            mesh.instance_buffer[current_frame].write(mesh.instances);
            mesh.instance_descriptor[current_frame].update(mesh.instance_buffer[current_frame].get_info(), vk::DescriptorType::eStorageBuffer, vulkan::DescriptorBinding::Instance);
        } else {
            mesh.instance_buffer[current_frame].write(mesh.instances);
        }
    }
} // namespace caelus::core