#ifndef CAELUS_WINDOW_HPP
#define CAELUS_WINDOW_HPP

#include "engine/Types.hpp"

struct GLFWwindow;

namespace vk {
    class Instance;
    class SurfaceKHR;
} // namespace vk

namespace caelus::engine::core::types::detail {
    struct VulkanData;
} // namespace caelus::engine::core::types::detail

namespace caelus::engine::core {
    class Window {
        GLFWwindow* window{};
    public:
        u32 width{}, height{};

        Window() = default;

        Window(const Window&) = delete;
        Window& operator =(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator =(Window&&) = delete;

        void create(const i32 width, const i32 h, const char* title);
        [[nodiscard]] vk::SurfaceKHR create_surface(const types::detail::VulkanData&) const;

        void poll_events() const;
        void close() const;
        [[nodiscard]] bool should_close() const;

        [[nodiscard]] GLFWwindow* handle() const;
    };
} // namespace caelus::engine::core

#endif //CAELUS_WINDOW_HPP
