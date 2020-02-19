#ifndef CAELUS_DESCRIPTORLAYOUTMANAGER_HPP
#define CAELUS_DESCRIPTORLAYOUTMANAGER_HPP

#include <unordered_map>
#include "vulkan/vulkan.hpp"
#include "engine/core/Types.hpp"

namespace caelus::core::manager {
    class DescriptorLayoutManager {
        static inline std::unordered_map<u32, vk::DescriptorSetLayout> layouts{};
    public:
        DescriptorLayoutManager() = default;

        static void add_layout(const u32, const vk::DescriptorSetLayout&);
        [[nodiscard]] static vk::DescriptorSetLayout& get_layout(const u32);
    };
} // namespace caelus::core::manager

#endif //CAELUS_DESCRIPTORLAYOUTMANAGER_HPP
