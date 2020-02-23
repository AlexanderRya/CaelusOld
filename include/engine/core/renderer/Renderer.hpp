#ifndef CAELUS_RENDERER_HPP
#define CAELUS_RENDERER_HPP

#include "engine/core/Types.hpp"

namespace caelus::core {
    namespace components {
        class Scene;
    } // namespace caelus::core::components

    namespace types::detail {
        struct VulkanContext;
    } // namespace caelus::core::types::detail

    class Renderer {
        u32 current_frame{};
        u32 image_index{};

        [[maybe_unused]] types::detail::VulkanContext& ctx;

    public:
        explicit Renderer(types::detail::VulkanContext& ctx) : ctx(ctx) {}

        void acquire_frame();
        void draw();
        void build(const components::Scene&);
    };
} // namespace caelus::core

#endif //CAELUS_RENDERER_HPP
