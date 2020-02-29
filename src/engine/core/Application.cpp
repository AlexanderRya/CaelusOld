#include "engine/core/components/buffers/GenericBuffer.hpp"
#include "engine/core/components/buffers/VertexBuffer.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/vulkan/DescriptorSet.hpp"
#include "engine/core/renderer/Renderer.hpp"
#include "engine/core/components/Scene.hpp"
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
            scene.layouts.emplace_back(vulkan::create_pipeline_layout(context, vulkan::PipelineLayoutType::GenericMesh));

            types::info::PipelineCreateInfo pipeline_create_info{}; {
                pipeline_create_info.ctx = &context;
                pipeline_create_info.type = vulkan::PipelineType::Default;
                pipeline_create_info.layout = scene.layouts[0];
                pipeline_create_info.vertex_path = "../resources/shaders/test.vert.spv";
                pipeline_create_info.fragment_path = "../resources/shaders/test.frag.spv";
            }

            scene.pipelines.emplace_back(vulkan::make_generic_mesh_pipeline(pipeline_create_info));

            types::info::VertexBufferCreateInfo buffer_create_info{}; {
                buffer_create_info.ctx = &context;
                buffer_create_info.vertices = util::generate_triangle_geometry();
            }

            scene.vertex_buffers.emplace_back(buffer_create_info);

            components::Mesh triangle{}; {
                triangle.pipeline = scene.pipelines[0];
                triangle.vertex_buffer_idx = 0;
                triangle.instances = {
                    glm::mat4(1.0f)
                };
            }

            components::create_mesh(triangle, context);

            scene.meshes.emplace_back(std::move(triangle));
        }

        while (!window.should_close()) {
            renderer.acquire_frame();
            renderer.build(scene);
            renderer.draw();
            window.poll_events();
        }
    }
} // namespace caelus::core