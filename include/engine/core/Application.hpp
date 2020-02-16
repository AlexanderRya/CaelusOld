#ifndef CAELUS_APPLICATION_HPP
#define CAELUS_APPLICATION_HPP

#include "details/ApplicationData.hpp"

namespace caelus::core {
    class Application {
        types::detail::ApplicationData data;

        void load_shaders();
        void load_meshes();

    public:
        Application() = default;

        void init();
        void run();
    };
} // namespace caelus::core

#endif //CAELUS_APPLICATION_HPP
