#ifndef CAELUS_APPLICATION_HPP
#define CAELUS_APPLICATION_HPP

#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/Window.hpp"

namespace caelus::core {
    class Application {
        Window window;
        types::detail::VulkanContext context;
    public:
        Application();
        ~Application();

        void init();
        void run();
    };
} // namespace caelus::core

#endif //CAELUS_APPLICATION_HPP
