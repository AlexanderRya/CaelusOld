#ifndef CAELUS_RENDERER_HPP
#define CAELUS_RENDERER_HPP

#include "engine/core/details/VulkanData.hpp"

namespace caelus::core {
    namespace types::detail {
        struct ApplicationData;
    } // namespace caelus::core::types::detail

    struct Renderer {
        types::detail::VulkanData vulkan_data;

        Renderer() = default;

        void init(const types::detail::ApplicationData&);
    };
} // namespace caelus::core

#endif //CAELUS_RENDERER_HPP
