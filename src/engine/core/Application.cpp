#include "engine/core/components/buffers/VertexBuffer.hpp"
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

        components::Scene scene{}; {
            types::info::PipelineCreateInfo pipeline_create_info{}; {
                pipeline_create_info.ctx = &context;
                pipeline_create_info.vertex_path = "../resources/shaders/test.vert.spv";
                pipeline_create_info.fragment_path = "../resources/shaders/test.frag.spv";
                pipeline_create_info.renderpass_index = 0;
                pipeline_create_info.subpass_index = 0;
            }

            scene.pipelines.emplace_back() = vulkan::make_generic_mesh_pipeline(pipeline_create_info);

            types::info::VertexBufferCreateInfo buffer_create_info{}; {
                buffer_create_info.ctx = &context;
                buffer_create_info.vertices = util::generate_triangle_geometry();
            }

            scene.vertex_buffers.emplace_back(buffer_create_info);

            scene.meshes.emplace_back(components::Mesh{
                .pipeline_idx = 0,
                .vertex_buffer_idx = 0,
                .vertex_count = 3,
                .instances_count = 1
            });

            scene_manager[0] = scene;
        }

        while (!window.should_close()) {
            renderer.acquire_frame();
            renderer.build(scene_manager.get_scene(0));
            renderer.draw();

            window.poll_events();
        }
    }
} // namespace caelus::core