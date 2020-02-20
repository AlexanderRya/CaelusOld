#ifndef CAELUS_DESCRIPTORSETMANAGER_HPP
#define CAELUS_DESCRIPTORSETMANAGER_HPP

#include <type_traits>
#include <unordered_map>

#include "vulkan/vulkan.hpp"
#include "engine/core/Types.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Constants.hpp"
#include "engine/core/info/DescriptorSetCreateInfo.hpp"
#include "engine/core/renderer/vulkan/DescriptorSet.hpp"

namespace caelus::core::manager {
    class DescriptorSetManager {
        struct DescriptorSetTypes {
            std::vector<vulkan::DescriptorSet<types::TransformUBO>> uniform_transform;
        };

        static inline std::unordered_map<u32, DescriptorSetTypes> descriptor_sets;
    public:
        DescriptorSetManager() = default;

        template <typename Ty>
        static void add_descriptor_sets(const u32, const types::info::DescriptorSetCreateInfo& create_info);

        template <typename Ty>
        static std::vector<vulkan::DescriptorSet<Ty>>& get_descriptor_sets(const u32);
    };

    template <typename Ty>
    void DescriptorSetManager::add_descriptor_sets(const u32 id, const types::info::DescriptorSetCreateInfo& create_info) {
        if (descriptor_sets.contains(id)) {
            logger::warning("Descriptor set with id: ", id, " already exists, overwriting.");
        }

        if constexpr (std::is_same_v<types::TransformUBO, Ty>) {
            descriptor_sets[id].uniform_transform =
                std::vector<vulkan::DescriptorSet<types::TransformUBO>>(constants::max_frames_in_flight);

            for (auto& set : descriptor_sets[id].uniform_transform) {
                set.create(create_info);
            }
        }
    }

    template <typename Ty>
    std::vector<vulkan::DescriptorSet<Ty>>& DescriptorSetManager::get_descriptor_sets(const u32 id) {
        if constexpr (std::is_same_v<types::TransformUBO, Ty>) {
            return descriptor_sets[id].uniform_transform;
        }
    }
} // namespace caelus::core::manager

#endif //CAELUS_DESCRIPTORSETMANAGER_HPP
