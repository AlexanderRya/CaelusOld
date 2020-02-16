#ifndef CAELUS_FRAMEBUFFER_HPP
#define CAELUS_FRAMEBUFFER_HPP

#include <vector>

namespace vk {
    class Framebuffer;
} // namespace vk

namespace caelus::engine::core::types::detail {
    struct VulkanData;
} // namespace caelus::engine::core::types::details

namespace caelus::engine::core {
    class Window;
} // namespace caelus::engine::core

namespace caelus::engine::core::vulkan {
    std::vector<vk::Framebuffer> get_framebuffers(const Window&, const types::detail::VulkanData&);
} // namespace caelus::engine::core::vulkan

#endif //CAELUS_FRAMEBUFFER_HPP
