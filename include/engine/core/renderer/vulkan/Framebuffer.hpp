#ifndef CAELUS_FRAMEBUFFER_HPP
#define CAELUS_FRAMEBUFFER_HPP

#include <vector>
#include "engine/core/Types.hpp"

namespace vk {
    class Framebuffer;
} // namespace vk

namespace caelus::core::types::detail {
    struct VulkanData;
} // namespace caelus::core::types::details

namespace caelus::core {
    class Window;
} // namespace caelus::core

namespace caelus::core::vulkan {
    std::vector<vk::Framebuffer> get_framebuffers(const u32, const Window&, const types::detail::VulkanData&);
} // namespace caelus::core::vulkan

#endif //CAELUS_FRAMEBUFFER_HPP
