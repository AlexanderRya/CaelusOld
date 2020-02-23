#ifndef CAELUS_APPLICATION_HPP
#define CAELUS_APPLICATION_HPP

#include "engine/core/components/managers/SceneManager.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/renderer/Renderer.hpp"
#include "engine/core/vulkan/Pipeline.hpp"
#include "engine/core/Window.hpp"

namespace caelus::core {
    namespace vulkan {
        struct PipelineLayout;
    } // namespace caelus::core::vulkan

    class Application {
        Window window;
        types::detail::VulkanContext context;

        std::vector<components::buffers::VertexBuffer> vertex_buffers;
        std::vector<vulkan::PipelineLayout> layouts;
        std::vector<vk::Pipeline> pipelines;
        components::managers::SceneManager scene_manager;

        void load_layouts();
        void load_pipelines();
        void load_scene();

    public:
        Application();
        ~Application();

        void init();
        void run();
    };
} // namespace caelus::core

#endif //CAELUS_APPLICATION_HPP
