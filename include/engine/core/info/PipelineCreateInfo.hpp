#ifndef CAELUS_PIPELINECREATEINFO_HPP
#define CAELUS_PIPELINECREATEINFO_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

#include <filesystem>

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
} // namespace caelus::core::types::info

#endif //CAELUS_PIPELINECREATEINFO_HPP
