#ifndef CAELUS_PIPELINELAYOUTMANAGER_HPP
#define CAELUS_PIPELINELAYOUTMANAGER_HPP

#include <unordered_map>
#include "vulkan/vulkan.hpp"
#include "engine/core/Types.hpp"

namespace caelus::core::manager {
    class PipelineLayoutManager {
        static inline std::unordered_map<u32, vk::PipelineLayout> layouts{};
    public:
        PipelineLayoutManager() = delete;

        static void add_layout(const u32, const vk::PipelineLayout&);
        static vk::PipelineLayout& get_layout(const u32);
    };
} // namespace caelus::core::manager

#endif //CAELUS_PIPELINELAYOUTMANAGER_HPP
