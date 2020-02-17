#ifndef CAELUS_RENDERER_HPP
#define CAELUS_RENDERER_HPP

#include "engine/core/details/VulkanData.hpp"

namespace caelus::core {
    class Window;

    namespace manager {
        class AssetManager;
    } // namespace caelus::core::manager

    class Renderer {
        u32 current_frame;
        u32 image_index;

        void record_buffers(const manager::AssetManager&);
    public:
        types::detail::VulkanData vulkan_data;

        Renderer() = default;

        void init(const Window&);
        void draw(const manager::AssetManager&);
    };
} // namespace caelus::core

#endif //CAELUS_RENDERER_HPP
