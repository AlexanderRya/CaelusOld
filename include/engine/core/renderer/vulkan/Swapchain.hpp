#ifndef CAELUS_SWAPCHAIN_HPP
#define CAELUS_SWAPCHAIN_HPP

#include "engine/core/Types.hpp"

namespace caelus::core::types::detail {
    struct SwapchainDetails;
} // namespace caelus::core::types::detail

namespace caelus::core {
    class Window;
} // namespace caelus::core

namespace caelus::core::vulkan {
    [[nodiscard]] types::detail::SwapchainDetails get_swapchain_details(const Window&, const types::detail::VulkanData&);
} // namespace caelus::core::vulkan

#endif //CAELUS_SWAPCHAIN_HPP
