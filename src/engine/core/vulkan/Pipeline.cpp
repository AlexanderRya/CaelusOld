#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/vulkan/Pipeline.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

#include <filesystem>
#include <fstream>

namespace caelus::core::vulkan {
    vk::ShaderModule load_module(const vk::Device& device, const std::filesystem::path& path) {
        std::ifstream in(path.generic_string(), std::fstream::binary);

        if (!in.is_open()) {
            throw std::runtime_error("Error, \"" + path.filename().generic_string() + "\" file not found.");
        }

        std::string spv{ std::istreambuf_iterator<char>{ in }, {} };

        vk::ShaderModuleCreateInfo create_info{}; {
            create_info.codeSize = spv.size();
            create_info.pCode = reinterpret_cast<const u32*>(spv.data());
        }

        auto module = device.createShaderModule(create_info);

        logger::info("Module \"" + path.filename().generic_string() + "\" successfully loaded");

        return module;
    }

    vk::Pipeline make_pipeline(const types::info::PipelineCreateInfo& info) {
        std::array<vk::ShaderModule, 2> modules{}; {
            modules[0] = load_module(info.ctx->device_details.device, info.vertex_path);
            modules[1] = load_module(info.ctx->device_details.device, info.fragment_path);
        }

        std::array<vk::PipelineShaderStageCreateInfo, 2> stages{}; {
            /* Vertex Module */ {
                vk::PipelineShaderStageCreateInfo vertex_stage{}; {
                    vertex_stage.pName = "main";
                    vertex_stage.module = modules[0];
                    vertex_stage.stage = vk::ShaderStageFlagBits::eVertex;
                }

                stages[0] = vertex_stage;
            }

            /* Fragment Module */ {
                vk::PipelineShaderStageCreateInfo fragment_stage{}; {
                    fragment_stage.pName = "main";
                    fragment_stage.module = modules[1];
                    fragment_stage.stage = vk::ShaderStageFlagBits::eFragment;
                }

                stages[1] = fragment_stage;
            }
        }

        const std::array dynamic_states{ vk::DynamicState::eViewport, vk::DynamicState::eScissor };

        vk::PipelineDynamicStateCreateInfo dynamic_state_create_info{}; {
            dynamic_state_create_info.dynamicStateCount = dynamic_states.size();
            dynamic_state_create_info.pDynamicStates = dynamic_states.data();
        }

        vk::GraphicsPipelineCreateInfo pipeline_info{}; {
            pipeline_info.stageCount = stages.size();
            pipeline_info.pStages = stages.data();
            pipeline_info.pVertexInputState = &info.vertex_input_info;
            pipeline_info.pInputAssemblyState = &info.input_assembly;
            pipeline_info.pViewportState = &info.viewport_state;
            pipeline_info.pRasterizationState = &info.rasterizer_state_info;
            pipeline_info.pMultisampleState = &info.multisampling_state_info;
            pipeline_info.pDepthStencilState = &info.depth_stencil_info;
            pipeline_info.pColorBlendState = &info.color_blend_info;
            pipeline_info.pDynamicState = &dynamic_state_create_info;
            pipeline_info.layout = info.pipeline_layout;
            pipeline_info.renderPass = info.ctx->render_passes[info.renderpass_index];
            pipeline_info.subpass = info.subpass_index;
            pipeline_info.basePipelineHandle = nullptr;
            pipeline_info.basePipelineIndex = -1;
        }

        auto graphics_pipeline = info.ctx->device_details.device.createGraphicsPipeline(nullptr, pipeline_info);

        logger::info("Pipeline successfully created");

        for (const auto& module : modules) {
            info.ctx->device_details.device.destroyShaderModule(module);
        }

        return graphics_pipeline;
    }

    vk::Pipeline make_default_pipeline(types::info::PipelineCreateInfo& info) {
        std::array<vk::ShaderModule, 2> modules{}; {
            modules[0] = load_module(info.ctx->device_details.device, info.vertex_path);
            modules[1] = load_module(info.ctx->device_details.device, info.fragment_path);
        }

        std::array<vk::PipelineShaderStageCreateInfo, 2> stages{}; {
            /* Vertex Module */ {
                vk::PipelineShaderStageCreateInfo vertex_stage{}; {
                    vertex_stage.pName = "main";
                    vertex_stage.module = modules[0];
                    vertex_stage.stage = vk::ShaderStageFlagBits::eVertex;
                }

                stages[0] = vertex_stage;
            }

            /* Fragment Module */ {
                vk::PipelineShaderStageCreateInfo fragment_stage{}; {
                    fragment_stage.pName = "main";
                    fragment_stage.module = modules[1];
                    fragment_stage.stage = vk::ShaderStageFlagBits::eFragment;
                }

                stages[1] = fragment_stage;
            }
        }

        const std::array dynamic_states{ vk::DynamicState::eViewport, vk::DynamicState::eScissor };

        vk::PipelineDynamicStateCreateInfo dynamic_state_create_info{}; {
            dynamic_state_create_info.dynamicStateCount = dynamic_states.size();
            dynamic_state_create_info.pDynamicStates = dynamic_states.data();
        }

        info.vertex_bindings.resize(1);
        info.vertex_bindings[0].stride = sizeof(types::Vertex);
        info.vertex_bindings[0].binding = 0;
        info.vertex_bindings[0].inputRate = vk::VertexInputRate::eVertex;

        info.vertex_attributes.resize(2);
        info.vertex_attributes[0].binding = 0;
        info.vertex_attributes[0].format = vk::Format::eR32G32B32Sfloat;
        info.vertex_attributes[0].location = 0;
        info.vertex_attributes[0].offset = offsetof(types::Vertex, pos);

        info.vertex_attributes[1].binding = 0;
        info.vertex_attributes[1].format = vk::Format::eR32G32Sfloat;
        info.vertex_attributes[1].location = 1;
        info.vertex_attributes[1].offset = offsetof(types::Vertex, tx_coords);

        info.vertex_input_info.pVertexBindingDescriptions = info.vertex_bindings.data();
        info.vertex_input_info.vertexBindingDescriptionCount = info.vertex_bindings.size();
        info.vertex_input_info.pVertexAttributeDescriptions = info.vertex_attributes.data();
        info.vertex_input_info.vertexAttributeDescriptionCount = info.vertex_attributes.size();

        info.input_assembly.topology = vk::PrimitiveTopology::eTriangleList;
        info.input_assembly.primitiveRestartEnable = false;

        vk::Viewport viewport{};
        vk::Rect2D scissor{};

        info.viewport_state.viewportCount = 1;
        info.viewport_state.pViewports = &viewport;
        info.viewport_state.scissorCount = 1;
        info.viewport_state.pScissors = &scissor;

        info.rasterizer_state_info.lineWidth = 1.0f;
        info.rasterizer_state_info.depthBiasEnable = false;
        info.rasterizer_state_info.depthClampEnable = false;
        info.rasterizer_state_info.rasterizerDiscardEnable = false;
        info.rasterizer_state_info.polygonMode = vk::PolygonMode::eFill;
        info.rasterizer_state_info.cullMode = vk::CullModeFlagBits::eNone;
        info.rasterizer_state_info.frontFace = vk::FrontFace::eCounterClockwise;

        info.multisampling_state_info.alphaToCoverageEnable = false;
        info.multisampling_state_info.sampleShadingEnable = false;
        info.multisampling_state_info.alphaToOneEnable = false;
        info.multisampling_state_info.rasterizationSamples = vk::SampleCountFlagBits::e1;
        info.multisampling_state_info.minSampleShading = 1.0f;
        info.multisampling_state_info.pSampleMask = nullptr;

        info.depth_stencil_info.stencilTestEnable = false;
        info.depth_stencil_info.depthTestEnable = true;
        info.depth_stencil_info.depthCompareOp = vk::CompareOp::eLess;

        info.color_blend_attachment.blendEnable = false;
        info.color_blend_attachment.colorWriteMask =
            vk::ColorComponentFlagBits::eR |
            vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB |
            vk::ColorComponentFlagBits::eA;
        info.color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eOne;
        info.color_blend_attachment.dstColorBlendFactor = vk::BlendFactor::eZero;
        info.color_blend_attachment.colorBlendOp = vk::BlendOp::eAdd;
        info.color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
        info.color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
        info.color_blend_attachment.alphaBlendOp = vk::BlendOp::eAdd;

        info.color_blend_info.attachmentCount = 1;
        info.color_blend_info.pAttachments = &info.color_blend_attachment;
        info.color_blend_info.logicOp = vk::LogicOp::eCopy;
        info.color_blend_info.logicOpEnable = false;
        info.color_blend_info.blendConstants[0] = 0.0f;
        info.color_blend_info.blendConstants[1] = 0.0f;
        info.color_blend_info.blendConstants[2] = 0.0f;
        info.color_blend_info.blendConstants[3] = 0.0f;

        vk::GraphicsPipelineCreateInfo pipeline_info{}; {
            pipeline_info.stageCount = stages.size();
            pipeline_info.pStages = stages.data();
            pipeline_info.pVertexInputState = &info.vertex_input_info;
            pipeline_info.pInputAssemblyState = &info.input_assembly;
            pipeline_info.pViewportState = &info.viewport_state;
            pipeline_info.pRasterizationState = &info.rasterizer_state_info;
            pipeline_info.pMultisampleState = &info.multisampling_state_info;
            pipeline_info.pDepthStencilState = &info.depth_stencil_info;
            pipeline_info.pColorBlendState = &info.color_blend_info;
            pipeline_info.pDynamicState = &dynamic_state_create_info;
            pipeline_info.layout = info.pipeline_layout;
            pipeline_info.renderPass = info.ctx->render_passes[info.renderpass_index];
            pipeline_info.subpass = info.subpass_index;
            pipeline_info.basePipelineHandle = nullptr;
            pipeline_info.basePipelineIndex = -1;
        }

        auto graphics_pipeline = info.ctx->device_details.device.createGraphicsPipeline(nullptr, pipeline_info);

        logger::info("Default pipeline successfully created");

        for (const auto& module : modules) {
            info.ctx->device_details.device.destroyShaderModule(module);
        }

        return graphics_pipeline;
    }
} // namespace caelus::core::vulkan