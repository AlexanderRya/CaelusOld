#ifndef CAELUS_RENDERPASS_HPP
#define CAELUS_RENDERPASS_HPP

namespace vk {
    class RenderPass;
} // namespace vk

namespace caelus::engine::core::types::detail {
    struct VulkanData;
} // namespace caelus::engine::core::types::detail

namespace caelus::engine::core::vulkan {
    [[nodiscard]] vk::RenderPass make_default_render_pass(const types::detail::VulkanData&);
} // namespace caelus::engine::core::vulkan

#endif //CAELUS_RENDERPASS_HPP
