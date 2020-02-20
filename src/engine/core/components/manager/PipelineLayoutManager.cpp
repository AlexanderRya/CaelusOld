#include "engine/core/components/manager/PipelineLayoutManager.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::manager {
    void PipelineLayoutManager::add_layout(
        const u32 id,
        const vk::PipelineLayout& pipeline_layout,
        const vk::DescriptorSetLayout& set_layout) {

        if (layouts.contains(id)) {
            logger::warning("Pipeline layout with id: ", id, " already exists, overwriting.");
        }

        layouts[id] = { pipeline_layout, set_layout };
    }

    types::detail::PipelineLayout& PipelineLayoutManager::get_layout(const u32 id) {
        return layouts.at(id);
    }
} // namespace caelus::core::manager