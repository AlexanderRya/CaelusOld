#ifndef CAELUS_PIPELINE_HPP
#define CAELUS_PIPELINE_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::types::info {
    struct PipelineCreateInfo;
} // namespace caelus::core::types::info

namespace caelus::core::vulkan {
    struct Pipeline {
        vk::Pipeline pipeline{};

        Pipeline() = default;

        void create(const types::info::PipelineCreateInfo&);
    };
} // namespace caelus::core::vulkan

#endif //CAELUS_PIPELINE_HPP
