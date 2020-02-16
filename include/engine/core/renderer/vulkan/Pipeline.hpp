#ifndef CAELUS_PIPELINE_HPP
#define CAELUS_PIPELINE_HPP

#include "engine/Types.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::engine::core::types::info {
    struct PipelineCreateInfo;
} // namespace caelus::engine::core::types::info

namespace caelus::engine::core::vulkan {
    struct Pipeline {
        vk::Pipeline pipeline{};

        Pipeline() = default;

        void create(const types::info::PipelineCreateInfo&);
    };
} // namespace caelus::engine::core::vulkan

#endif //CAELUS_PIPELINE_HPP
