#include "engine/core/components/manager/DescriptorSetManager.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::manager {
    void DescriptorSetManager::add_descriptor_set(const u32 id, const DescriptorSetManager::descriptor_type& descriptor) {
        if (descriptor_sets.contains(id)) {
            logger::warning("Descriptor set with id: ", id, " already exists, overwriting.");
        }

        descriptor_sets[id] = descriptor;
    }

    DescriptorSetManager::descriptor_type& DescriptorSetManager::get_descriptor_set(const u32 id) {
        return descriptor_sets.at(id);
    }
} // namespace caelus::core::manager