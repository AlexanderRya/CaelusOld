#ifndef CAELUS_APPLICATION_HPP
#define CAELUS_APPLICATION_HPP

#include "engine/core/Window.hpp"
#include "engine/core/renderer/Renderer.hpp"
#include "engine/core/components/manager/ResourceManager.hpp"
#include "engine/core/components/manager/PipelineLayoutManager.hpp"

namespace caelus::core {
    class Application {
        Window window;
        Renderer renderer;
        manager::ResourceManager resource_manager;
        manager::PipelineLayoutManager pipeline_layout_manager;


        void load_shaders();
        void load_meshes();

    public:
        Application() = default;

        void init();
        void run();
    };
} // namespace caelus::core

#endif //CAELUS_APPLICATION_HPP
