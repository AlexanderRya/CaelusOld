#ifndef CAELUS_APPLICATION_HPP
#define CAELUS_APPLICATION_HPP

#include "engine/core/components/managers/SceneManager.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/renderer/Renderer.hpp"
#include "engine/core/components/Scene.hpp"
#include "engine/core/vulkan/Pipeline.hpp"
#include "engine/core/Window.hpp"

namespace caelus::core {
    class Application {
        Window window;
        types::detail::VulkanContext context;

        components::managers::SceneManager scene_manager;
    public:
        Application();
        ~Application();

        void init();
        void run();
    };
} // namespace caelus::core

#endif //CAELUS_APPLICATION_HPP
