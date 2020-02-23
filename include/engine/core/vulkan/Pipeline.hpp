#ifndef CAELUS_PIPELINE_HPP
#define CAELUS_PIPELINE_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

#include <filesystem>

namespace caelus::core::types {
    namespace detail {
        struct VulkanContext;
    } // // namespace caelus::core::types::detail

    namespace info {
        enum class PipelineType {
            Default,
            Custom
        };

        struct PipelineCreateInfo {
            std::vector<vk::VertexInputBindingDescription> vertex_bindings{};
            std::vector<vk::VertexInputAttributeDescription> vertex_attributes{};
            vk::PipelineVertexInputStateCreateInfo vertex_input_info{};
            vk::PipelineInputAssemblyStateCreateInfo input_assembly{};
            vk::PipelineViewportStateCreateInfo viewport_state{};
            vk::PipelineRasterizationStateCreateInfo rasterizer_state_info{};
            vk::PipelineMultisampleStateCreateInfo multisampling_state_info{};
            vk::PipelineDepthStencilStateCreateInfo depth_stencil_info{};
            vk::PipelineColorBlendAttachmentState color_blend_attachment{};
            vk::PipelineColorBlendStateCreateInfo color_blend_info{};

            vk::PipelineLayout pipeline_layout{};

            PipelineType type = PipelineType::Default;

            std::filesystem::path vertex_path{};
            std::filesystem::path fragment_path{};

            usize renderpass_index = 0;
            usize subpass_index{};

            const detail::VulkanContext* ctx;
        };
    } // namespace caelus::core::types::info
} // namespace caelus::core::types

namespace caelus::core::vulkan {
    [[nodiscard]] vk::ShaderModule load_module(const vk::Device&, const std::filesystem::path&);
    [[nodiscard]] vk::Pipeline make_pipeline(const types::info::PipelineCreateInfo&);
    [[nodiscard]] vk::Pipeline make_default_pipeline(types::info::PipelineCreateInfo&);
} // namespace caelus::core::vulkan

#endif //CAELUS_PIPELINE_HPP
