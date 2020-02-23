#include "engine/core/vulkan/PipelineLayout.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/components/Mesh.hpp"
#include "engine/core/vulkan/Pipeline.hpp"
#include "engine/core/Application.hpp"
#include "engine/core/util/Util.hpp"

namespace caelus::core {
    Application::Application() = default;
    Application::~Application() = default;

    void Application::init() {
        window.create(1280, 720, "Engine");
        context = vulkan::make_vulkan_context(window);
    }

    void Application::run() {
        Renderer renderer{ context };

        load_layouts();
        load_pipelines();
        load_scene();

        while (!window.should_close()) {
            renderer.acquire_frame();
            renderer.build(scene_manager[0]);
            renderer.draw();
            window.poll_events();
        }
    }

    void Application::load_layouts() {
        layouts.resize(1);
        /* Default layout */ {
            vk::PipelineLayoutCreateInfo pipeline_layout_create_info{}; {
                pipeline_layout_create_info.pushConstantRangeCount = 0;
                pipeline_layout_create_info.pPushConstantRanges = nullptr;
                pipeline_layout_create_info.setLayoutCount = 0;
                pipeline_layout_create_info.pSetLayouts = nullptr;
            }

            layouts[0].pipeline_layout = context.device_details.device.createPipelineLayout(pipeline_layout_create_info);
            layouts[0].set_layout = {};
        }
    }

    void Application::load_pipelines() {
        /* Generic pipeline*/ {
            types::info::PipelineCreateInfo info{}; {
                info.ctx = &context;
                info.pipeline_layout = layouts[0].pipeline_layout;

                info.renderpass_index = 0;
                info.subpass_index = 0;

                info.fragment_path = "../resources/shaders/test.frag.spv";
                info.vertex_path = "../resources/shaders/test.vert.spv";
            }

            pipelines.emplace_back(vulkan::make_default_pipeline(info));
        }
    }

    void Application::load_scene() {
        components::Scene scene{};

        vertex_buffers.emplace_back(context).allocate(util::generate_triangle_geometry());

        components::Mesh mesh{}; {
            mesh.pipeline = pipelines[0];
            mesh.vertex_buffer = vertex_buffers[0].buffer;
            mesh.vertex_memory = vertex_buffers[0].memory;
            mesh.vertex_count = 3;
        }

        scene.add_mesh(mesh);
        scene_manager.add_scene(0, scene);
    }
} // namespace caelus::core