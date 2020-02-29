#ifndef CAELUS_PIPELINELAYOUT_HPP
#define CAELUS_PIPELINELAYOUT_HPP

#include "vulkan/vulkan.hpp"
#include <vector>

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    enum class PipelineLayoutType {
        GenericMesh
    };

    struct PipelineLayout {
        PipelineLayoutType type{};

        vk::PipelineLayout pipeline_layout{};
        vk::DescriptorSetLayout set_layout{};
    };

    [[nodiscard]] PipelineLayout create_pipeline_layout(const types::detail::VulkanContext&, const PipelineLayoutType);
} // namespace caelus::core::vulkan

#endif //CAELUS_PIPELINELAYOUT_HPP
