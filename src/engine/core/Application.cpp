#include "engine/core/components/buffers/VertexBuffer.hpp"
#include "engine/core/renderer/vulkan/Pipeline.hpp"
#include "engine/core/info/PipelineCreateInfo.hpp"
#include "engine/core/components/Mesh.hpp"
#include "engine/core/Application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace caelus::core {
    void Application::init() {
        window.create(1280, 720, "Engine");
        renderer.init(window);
        load_shaders();
        load_meshes();
    }

    void Application::load_shaders() {
        const auto& data = renderer.vulkan_data;

        /* Load layouts */ {
            vk::PipelineLayoutCreateInfo pipeline_layout{}; {
                pipeline_layout.pushConstantRangeCount = 0;
                pipeline_layout.pPushConstantRanges = nullptr;
                pipeline_layout.setLayoutCount = 0;
                pipeline_layout.pSetLayouts = nullptr;
            }

            pipeline_layout_manager.add_layout(0, data.device_details.device.createPipelineLayout(pipeline_layout));
        }

        /* test.spv */ {
            types::info::PipelineCreateInfo info{}; {
                info.subpass_index = 0;
                info.id = 0;
                info.device = data.device_details.device;
                info.render_pass = data.render_passes[0];

                info.vertex_path = "../resources/shaders/test.vert.spv";
                info.fragment_path = "../resources/shaders/test.frag.spv";

                info.pipeline_layout = pipeline_layout_manager.get_layout(0);
            }

            resource_manager.add_pipeline(info);
        }
    }

    void Application::run() {
        while (!window.should_close()) {
            renderer.draw(resource_manager);
            window.poll_events();
        }
    }

    void Application::load_meshes() {
        components::Mesh triangle{}; {
            triangle.pipeline_id = 0;
            triangle.instances = {
                glm::translate(glm::mat4(1.0f), glm::vec3(.5f, 0.0f, 0.0f)),
                glm::translate(glm::mat4(1.0f), glm::vec3(-.5f, 0.0f, 0.0f)),
            };
            triangle.vertices = {
                { { 0.0f, -0.5f, 0.0f }, {} },
                { { -0.5f, 0.5f, 0.0f }, {} },
                { { 0.5f, 0.5f, 0.0f }, {} },
            };
            triangle.vertex_buffer.allocate(triangle.vertices, renderer.vulkan_data);
            triangle.instance_buffer.allocate(triangle.instances, renderer.vulkan_data);
        }

        resource_manager.add_mesh(triangle);
    }
} // namespace caelus::core