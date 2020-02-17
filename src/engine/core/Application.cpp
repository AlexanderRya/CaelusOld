#include "engine/core/renderer/vulkan/Pipeline.hpp"
#include "engine/core/info/PipelineCreateInfo.hpp"
#include "engine/core/Application.hpp"

namespace caelus::core {
    void Application::init() {
        window.create(1280, 720, "Engine");
        renderer.init(window);
        load_shaders();
    }

    void Application::load_shaders() {
        const auto& data = renderer.vulkan_data;
        /* test.spv */ {
            vk::PipelineLayoutCreateInfo pipeline_layout_create_info{}; {
                pipeline_layout_create_info.pushConstantRangeCount = 0;
                pipeline_layout_create_info.pPushConstantRanges = nullptr;
                pipeline_layout_create_info.setLayoutCount = 0;
                pipeline_layout_create_info.pSetLayouts = nullptr;
            }

            types::info::PipelineCreateInfo info{}; {
                info.subpass_index = 0;
                info.id = 0;
                info.device = data.device_details.device;
                info.render_pass = data.render_passes[0];

                info.vertex_path = "../resources/shaders/test.vert.spv";
                info.fragment_path = "../resources/shaders/test.frag.spv";

                info.pipeline_layout = data.device_details.device.createPipelineLayout(pipeline_layout_create_info);
            }

            asset_manager.add_pipeline(info);
        }
    }

    void Application::run() {
        while (!window.should_close()) {
            renderer.draw(asset_manager);
            window.poll_events();
        }
    }
} // namespace caelus::core