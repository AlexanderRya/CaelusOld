#include "engine/core/renderer/vulkan/Pipeline.hpp"
#include "engine/core/info/PipelineCreateInfo.hpp"
#include "engine/core/Application.hpp"

namespace caelus::core {
    void Application::init() {
        data.window.create(1280, 720, "Engine");
        data.renderer.init(data);
        load_shaders();
    }

    void Application::load_shaders() {
        const auto& vulkan_data = data.renderer.vulkan_data;

        vk::PipelineLayoutCreateInfo pipeline_layout_create_info{}; {
            pipeline_layout_create_info.pushConstantRangeCount = 0;
            pipeline_layout_create_info.pPushConstantRanges = nullptr;
            pipeline_layout_create_info.setLayoutCount = 0;
            pipeline_layout_create_info.pSetLayouts = nullptr;
        }

        vk::Viewport viewport{}; {
            viewport.width = vulkan_data.swapchain_details.extent.width;
            viewport.height = vulkan_data.swapchain_details.extent.height;
            viewport.x = 0;
            viewport.y = 0;
            viewport.maxDepth = 1.0f;
            viewport.minDepth = 0.0f;
        }

        vk::Rect2D scissor{}; {
            scissor.extent = vulkan_data.swapchain_details.extent;
            scissor.offset = { { 0, 0 } };
        }

        vk::PipelineViewportStateCreateInfo viewport_state_info{}; {
            viewport_state_info.viewportCount = 1;
            viewport_state_info.pViewports = &viewport;
            viewport_state_info.scissorCount = 1;
            viewport_state_info.pScissors = &scissor;
        }

        /* test.spv */ {
            types::info::PipelineCreateInfo info{}; {
                info.subpass_index = 0;
                info.device = vulkan_data.device_details.device;
                info.render_pass = vulkan_data.render_passes[0];

                info.vertex_path = "../resources/shaders/test.vert.spv";
                info.fragment_path = "../resources/shaders/test.frag.spv";

                info.pipeline_layout = vulkan_data.device_details.device.createPipelineLayout(pipeline_layout_create_info);
                info.viewport_state_info = viewport_state_info;
            }

            data.pipelines[0].create(info);
        }
    }

    void Application::run() {
        while (!data.window.should_close()) {
            data.window.poll_events();
        }
    }
} // namespace caelus::core