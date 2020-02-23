#ifndef CAELUS_PIPELINELAYOUT_HPP
#define CAELUS_PIPELINELAYOUT_HPP

#include "vulkan/vulkan.hpp"
#include <vector>

namespace caelus::core::vulkan {
    struct PipelineLayout {
        vk::PipelineLayout pipeline_layout{};
        std::vector<vk::DescriptorSetLayout> set_layout{};
    };
} // namespace caelus::core::vulkan

#endif //CAELUS_PIPELINELAYOUT_HPP
