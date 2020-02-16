#ifndef CAELUS_RENDERPASS_HPP
#define CAELUS_RENDERPASS_HPP

namespace vk {
    class RenderPass;
} // namespace vk

namespace caelus::core::types::detail {
    struct VulkanData;
} // namespace caelus::core::types::detail

namespace caelus::core::vulkan {
    [[nodiscard]] vk::RenderPass make_default_render_pass(const types::detail::VulkanData&);
} // namespace caelus::core::vulkan

#endif //CAELUS_RENDERPASS_HPP
