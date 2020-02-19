#ifndef CAELUS_DESCRIPTORSETMANAGER_HPP
#define CAELUS_DESCRIPTORSETMANAGER_HPP

#include <unordered_map>
#include "vulkan/vulkan.hpp"
#include "engine/core/Types.hpp"
#include "engine/core/Constants.hpp"
#include "engine/core/renderer/vulkan/DescriptorSet.hpp"

namespace caelus::core::manager {
    class DescriptorSetManager {
        using descriptor_type = std::array<vulkan::DescriptorSet, constants::max_frames_in_flight>;
        static inline std::unordered_map<u32, descriptor_type> descriptor_sets{};
    public:
        DescriptorSetManager() = default;

        static void add_descriptor_set(const u32, const descriptor_type&);
        [[nodiscard]] static descriptor_type& get_descriptor_set(const u32);
    };
} // namespace caelus::core::manager

#endif //CAELUS_DESCRIPTORSETMANAGER_HPP
