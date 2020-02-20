#include "engine/core/components/manager/DescriptorLayoutManager.hpp"
#include "engine/core/components/manager/PipelineLayoutManager.hpp"
#include "engine/core/components/manager/DescriptorSetManager.hpp"
#include "engine/core/components/manager/ResourceManager.hpp"
#include "engine/core/components/buffers/VertexBuffer.hpp"
#include "engine/core/renderer/vulkan/DescriptorSet.hpp"
#include "engine/core/info/DescriptorSetCreateInfo.hpp"
#include "engine/core/info/PipelineCreateInfo.hpp"
#include "engine/core/components/Mesh.hpp"
#include "engine/core/Application.hpp"
#include "engine/core/Types.hpp"

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
        const auto& ctx = renderer.context;

        /* Load descriptor set layouts */ {
            vk::DescriptorSetLayoutBinding layout_binding{}; {
                layout_binding.binding = 0;
                layout_binding.descriptorType = vk::DescriptorType::eUniformBuffer;
                layout_binding.descriptorCount = 1;
                layout_binding.pImmutableSamplers = nullptr;
                layout_binding.stageFlags = vk::ShaderStageFlagBits::eVertex;
            }

            vk::DescriptorSetLayoutCreateInfo descriptor_set_layout{}; {
                descriptor_set_layout.bindingCount = 1;
                descriptor_set_layout.pBindings = &layout_binding;
            }

            manager::DescriptorLayoutManager::add_layout(0, ctx.device_details.device.createDescriptorSetLayout(descriptor_set_layout));
        }

        /* Load descriptor sets */ {
            types::info::DescriptorSetCreateInfo set_create_info{}; {
                set_create_info.layout_id = 0;
                set_create_info.binding = 0;
                set_create_info.ctx = &ctx;
                set_create_info.descriptor_type = vk::DescriptorType::eUniformBuffer;
                set_create_info.usage_flags = vk::BufferUsageFlagBits::eUniformBuffer;
            }

            manager::DescriptorSetManager::add_descriptor_sets<types::TransformUBO>(0, set_create_info);
        }

        /* Load pipeline layouts */ {
            vk::PipelineLayoutCreateInfo pipeline_layout{}; {
                pipeline_layout.pushConstantRangeCount = 0;
                pipeline_layout.pPushConstantRanges = nullptr;
                pipeline_layout.setLayoutCount = 1;
                pipeline_layout.pSetLayouts = &manager::DescriptorLayoutManager::get_layout(0);
            }

            manager::PipelineLayoutManager::add_layout(0, ctx.device_details.device.createPipelineLayout(pipeline_layout));
        }

        /* test.spv */ {
            types::info::PipelineCreateInfo info{}; {
                info.subpass_index = 0;
                info.id = 0;
                info.device = ctx.device_details.device;
                info.render_pass = ctx.render_passes[0];

                info.vertex_path = "../resources/shaders/test.vert.spv";
                info.fragment_path = "../resources/shaders/test.frag.spv";

                info.pipeline_layout = manager::PipelineLayoutManager::get_layout(0);
            }

            manager::ResourceManager::add_pipeline(info);
        }
    }

    void Application::run() {
        while (!window.should_close()) {
            renderer.draw();
            window.poll_events();
        }
    }

    void Application::load_meshes() {
        components::Mesh triangle{}; {
            triangle.pipeline_id = 0;
            triangle.instances = {
                glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
            };
            triangle.vertices = {
                { { 0.0f, -0.5f, 0.0f }, {} },
                { { -0.26f, 0.5f, 0.0f }, {} },
                { { 0.26f, 0.5f, 0.0f }, {} },
            };
            triangle.vertex_buffer.allocate(triangle.vertices, renderer.context);
            triangle.instance_buffer.allocate(triangle.instances, renderer.context);
        }

        manager::ResourceManager::add_mesh(triangle);
    }
} // namespace caelus::core