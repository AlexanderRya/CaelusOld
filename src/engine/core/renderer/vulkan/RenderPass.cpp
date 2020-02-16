#include "engine/core/renderer/vulkan/RenderPass.hpp"
#include "engine/core/details/VulkanData.hpp"
#include "engine/logger/Logger.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::vulkan {
    vk::RenderPass make_default_render_pass(const types::detail::VulkanData& data) {
        vk::AttachmentDescription attachment_description{}; {
            attachment_description.format = data.swapchain_details.format.format;
            attachment_description.samples = vk::SampleCountFlagBits::e1;
            attachment_description.loadOp = vk::AttachmentLoadOp::eClear;
            attachment_description.storeOp = vk::AttachmentStoreOp::eStore;
            attachment_description.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
            attachment_description.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
            attachment_description.initialLayout = vk::ImageLayout::eUndefined;
            attachment_description.finalLayout = vk::ImageLayout::ePresentSrcKHR;
        }

        vk::AttachmentReference color_attachment{}; {
            color_attachment.layout = vk::ImageLayout::eColorAttachmentOptimal;
            color_attachment.attachment = 0;
        }

        vk::SubpassDescription subpass_description{}; {
            subpass_description.colorAttachmentCount = 1;
            subpass_description.pColorAttachments = &color_attachment;
            subpass_description.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        }

        vk::SubpassDependency subpass_dependency{}; {
            subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpass_dependency.dstSubpass = 0;
            subpass_dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
            subpass_dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
            subpass_dependency.srcAccessMask = {};
            subpass_dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
        }

        vk::RenderPassCreateInfo render_pass_create_info{}; {
            render_pass_create_info.attachmentCount = 1;
            render_pass_create_info.pAttachments = &attachment_description;
            render_pass_create_info.subpassCount = 1;
            render_pass_create_info.pSubpasses = &subpass_description;
            render_pass_create_info.dependencyCount = 1;
            render_pass_create_info.pDependencies = &subpass_dependency;
        }

        auto render_pass = data.device_details.device.createRenderPass(render_pass_create_info);

        logger::info("Default renderpass successfully created, index: ", 0);

        return render_pass;
    }
} // namespace caelus::core::vulkan