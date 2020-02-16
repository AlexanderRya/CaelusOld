#ifndef CAELUS_APPLICATIONDATA_HPP
#define CAELUS_APPLICATIONDATA_HPP

#include "engine/core/Window.hpp"
#include "engine/core/renderer/Renderer.hpp"
#include "engine/core/renderer/vulkan/Pipeline.hpp"

namespace caelus::engine::core::types::detail {
    struct ApplicationData {
        Window window;
        Renderer renderer;

        std::array<vulkan::Pipeline, 1> pipelines{};
    };
} // namespace caelus::engine::core::types::detail

#endif //CAELUS_APPLICATIONDATA_HPP
