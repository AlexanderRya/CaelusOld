#ifndef CAELUS_RENDERPASS_HPP
#define CAELUS_RENDERPASS_HPP

namespace vk {
    class RenderPass;
} // namespace vk

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::types::info {
    struct RenderPassCreateInfo {

    };
}

namespace caelus::core::vulkan {
    [[nodiscard]] vk::RenderPass make_default_render_pass(const types::detail::VulkanContext&);
    [[nodiscard]] vk::RenderPass make_render_pass(const types::detail::VulkanContext&);
} // namespace caelus::core::vulkan

#endif //CAELUS_RENDERPASS_HPP
