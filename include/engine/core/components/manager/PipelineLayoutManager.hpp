#ifndef CAELUS_PIPELINELAYOUTMANAGER_HPP
#define CAELUS_PIPELINELAYOUTMANAGER_HPP

#include <unordered_map>
#include "vulkan/vulkan.hpp"
#include "engine/core/Types.hpp"

namespace caelus::core::manager {
    class PipelineLayoutManager {
        std::unordered_map<u32, vk::PipelineLayout> layouts;
    public:
        PipelineLayoutManager() = default;

        void add_layout(const u32, const vk::PipelineLayout&);
        vk::PipelineLayout get_layout(const u32) const;
    };
} // namespace caelus::core::manager

#endif //CAELUS_PIPELINELAYOUTMANAGER_HPP
