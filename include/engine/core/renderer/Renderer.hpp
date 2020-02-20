#ifndef CAELUS_RENDERER_HPP
#define CAELUS_RENDERER_HPP

#include "engine/core/details/VulkanContext.hpp"

namespace caelus::core {
    class Window;
    namespace vulkan {
        struct DescriptorSet;
    } // namespace caelus::core::vulkan

    class Renderer {
        u32 current_frame{};
        u32 image_index{};

        void record_buffers();
        void update_sets(const std::vector<vulkan::DescriptorSet>& descriptor_sets);
    public:
        types::detail::VulkanContext context;

        Renderer() = default;

        void init(const Window&);
        void draw();
    };
} // namespace caelus::core

#endif //CAELUS_RENDERER_HPP
