#ifndef CAELUS_SEMAPHORE_HPP
#define CAELUS_SEMAPHORE_HPP

#include "engine/core/Types.hpp"
#include <vector>

namespace vk {
    class Semaphore;
} // namespace vk

namespace caelus::core::types::detail {
    struct VulkanData;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    [[nodiscard]] std::vector<vk::Semaphore> make_semaphores(const u32, const types::detail::VulkanData&);
} // namespace caelus::core::vulkan

#endif //CAELUS_SEMAPHORE_HPP
