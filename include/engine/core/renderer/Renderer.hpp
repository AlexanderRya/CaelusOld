#ifndef CAELUS_RENDERER_HPP
#define CAELUS_RENDERER_HPP

#include "engine/core/Types.hpp"

namespace caelus::core {
    namespace components {
        struct Scene;
        struct Mesh;
    } // namespace caelus::core::components

    namespace types::detail {
        struct VulkanContext;
    } // namespace caelus::core::types::detail

    class Renderer {
        u32 current_frame{};
        u32 image_index{};

        std::vector<vk::Fence> frames_in_flight;

        const types::detail::VulkanContext& ctx;

        void update_mesh(components::Mesh&);
    public:
        explicit Renderer(const types::detail::VulkanContext& ctx);

        u32 acquire_frame();
        void draw();
        void build(components::Scene&);
    };
} // namespace caelus::core

#endif //CAELUS_RENDERER_HPP
