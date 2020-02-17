#ifndef CAELUS_WINDOW_HPP
#define CAELUS_WINDOW_HPP

#include "Types.hpp"

struct GLFWwindow;

namespace vk {
    class Instance;
    class SurfaceKHR;
} // namespace vk

namespace caelus::core::types::detail {
    struct VulkanData;
} // namespace caelus::core::types::detail

namespace caelus::core {
    class Window {
        GLFWwindow* window{};
    public:
        static inline bool resized = true;
        u32 width{}, height{};

        Window() = default;

        Window(const Window&) = delete;
        Window& operator =(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator =(Window&&) = delete;

        void create(const i32, const i32, const char*);
        [[nodiscard]] vk::SurfaceKHR create_surface(const types::detail::VulkanData&) const;

        void poll_events() const;
        void close() const;
        [[nodiscard]] bool should_close() const;

        [[nodiscard]] GLFWwindow* handle() const;
    };
} // namespace caelus::core

#endif //CAELUS_WINDOW_HPP
