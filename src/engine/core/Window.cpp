#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/Callbacks.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Window.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <stdexcept>

namespace caelus::core {
    void Window::create(const i32 w, const i32 h, const char* title) {
        glfwSetErrorCallback(callbacks::glfw_error_callback);

        if (!glfwInit()) {
            throw std::runtime_error("Failed glfw init");
        }

        caelus::logger::info("glfwInit() success");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        width = w;
        height = h;

        if (!(window = glfwCreateWindow(w, h, title, nullptr, nullptr))) {
            throw std::runtime_error("Failed window creation");
        }

        caelus::logger::info("Window successfully created with size: ", width, "x", height);
    }

    vk::SurfaceKHR Window::create_surface(const types::detail::VulkanContext& ctx) const {
        vk::SurfaceKHR surface;

        glfwCreateWindowSurface(
            ctx.instance,
            window,
            nullptr,
            reinterpret_cast<VkSurfaceKHR*>(&surface));

        return surface;
    }

    void Window::poll_events() const {
        glfwPollEvents();
    }

    bool Window::should_close() const {
        return glfwWindowShouldClose(window);
    }

    void Window::close() const {
        glfwSetWindowShouldClose(window, true);
    }
} // namespace caelus::core