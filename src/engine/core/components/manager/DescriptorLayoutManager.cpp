#include "engine/core/components/manager/DescriptorLayoutManager.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::manager {
    void DescriptorLayoutManager::add_layout(const u32 id, const vk::DescriptorSetLayout& layout) {
        if (layouts.contains(id)) {
            logger::warning("Descriptor set layout with id: ", id, " already exists, overwriting.");
        }

        layouts[id] = layout;
    }

    vk::DescriptorSetLayout& DescriptorLayoutManager::get_layout(const u32 id) {
        return layouts.at(id);
    }
} // namespace caelus::core::manager