#include "engine/core/components/manager/PipelineLayoutManager.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::manager {
    void PipelineLayoutManager::add_layout(const u32 id, const vk::PipelineLayout& layout) {
        if (layouts.contains(id)) {
            logger::warning("Pipeline layout with id: ", id, " already exists, overwriting.");
        }

        layouts[id] = layout;
    }

    vk::PipelineLayout PipelineLayoutManager::get_layout(const u32 id) const {
        return layouts.at(id);
    }
} // namespace caelus::core::manager