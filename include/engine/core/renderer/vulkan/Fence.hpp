#ifndef CAELUS_FENCE_HPP
#define CAELUS_FENCE_HPP

#include "engine/core/Types.hpp"
#include <vector>

namespace vk {
    class Fence;
} // namespace vk

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    [[nodiscard]] std::vector<vk::Fence> make_fences(const u32, const types::detail::VulkanContext&);
    [[nodiscard]] vk::Fence make_fence(const types::detail::VulkanContext&);
} // namespace caelus::core::vulkan

#endif //CAELUS_FENCE_HPP
