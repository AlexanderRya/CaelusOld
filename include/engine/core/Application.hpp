#ifndef CAELUS_APPLICATION_HPP
#define CAELUS_APPLICATION_HPP

#include "details/ApplicationData.hpp"

namespace caelus::engine::core {
    class Application {
        types::detail::ApplicationData data;

        void load_shaders();
        void load_meshes();

    public:
        Application() = default;

        void init(const u32 width, const u32 heigtht, const char* title);
        void run();
    };
} // namespace caelus::engine::core

#endif //CAELUS_APPLICATION_HPP
