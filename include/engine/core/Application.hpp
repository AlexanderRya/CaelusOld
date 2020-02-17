#ifndef CAELUS_APPLICATION_HPP
#define CAELUS_APPLICATION_HPP

#include "engine/core/Window.hpp"
#include "engine/core/renderer/Renderer.hpp"
#include "engine/core/components/manager/AssetManager.hpp"

namespace caelus::core {
    class Application {
        Window window;
        Renderer renderer;
        manager::AssetManager asset_manager;

        void load_shaders();
        void load_meshes();

    public:
        Application() = default;

        void init();
        void run();
    };
} // namespace caelus::core

#endif //CAELUS_APPLICATION_HPP
