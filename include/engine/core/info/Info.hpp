#ifndef CAELUS_INFO_HPP
#define CAELUS_INFO_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

#include <filesystem>

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::types::info {
    struct PipelineCreateInfo {
        vk::PipelineVertexInputStateCreateInfo vertex_input_info{};
        vk::PipelineInputAssemblyStateCreateInfo input_assembly{};
        vk::PipelineViewportStateCreateInfo viewport_state{};
        vk::PipelineRasterizationStateCreateInfo rasterizer_state_info{};
        vk::PipelineMultisampleStateCreateInfo multisampling_state_info{};
        vk::PipelineDepthStencilStateCreateInfo depth_stencil_info{};
        vk::PipelineColorBlendAttachmentState color_blend_attachment{};
        vk::PipelineColorBlendStateCreateInfo color_blend_info{};
        vk::PipelineLayout pipeline_layout{};

        std::filesystem::path vertex_path{};
        std::filesystem::path fragment_path{};

        vk::Device device{};
        vk::RenderPass render_pass{};
        u64 subpass_index{};
        u32 id = -1;

        PipelineCreateInfo();
    };

    struct DescriptorSetInfo {
        const types::detail::VulkanContext* ctx;

        vk::DescriptorType type;

        u32 binding;
        u32 buffer_id;
        u32 layout_id;
    };
} // namespace caelus::core::types::info

#endif //CAELUS_INFO_HPP
