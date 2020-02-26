#ifndef CAELUS_PIPELINE_HPP
#define CAELUS_PIPELINE_HPP

#include "engine/core/vulkan/PipelineLayout.hpp"
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
            usize renderpass_index = 0;
            usize subpass_index = 0;

            PipelineType type = PipelineType::Default;

            std::filesystem::path vertex_path{};
            std::filesystem::path fragment_path{};

            const detail::VulkanContext* ctx;
        };
    } // namespace caelus::core::types::info
} // namespace caelus::core::types

namespace caelus::core::vulkan {
    struct Pipeline {
        vk::Pipeline pipeline;
        vulkan::PipelineLayout layout;
    };

    [[nodiscard]] vk::ShaderModule load_module(const types::detail::VulkanContext*, const std::filesystem::path&);
    [[nodiscard]] Pipeline make_generic_mesh_pipeline(const types::info::PipelineCreateInfo&);
} // namespace caelus::core::vulkan

#endif //CAELUS_PIPELINE_HPP
