#ifndef CAELUS_SWAPCHAIN_HPP
#define CAELUS_SWAPCHAIN_HPP

#include "engine/Types.hpp"

namespace caelus::engine::core::types::detail {
    struct SwapchainDetails;
} // namespace caelus::engine::core::types::detail

namespace caelus::engine::core {
    class Window;
} // namespace caelus::engine::core

namespace caelus::engine::core::vulkan {
    [[nodiscard]] types::detail::SwapchainDetails get_swapchain_details(const Window&, const types::detail::VulkanData&);
} // namespace caelus::engine::core::vulkan

#endif //CAELUS_SWAPCHAIN_HPP
