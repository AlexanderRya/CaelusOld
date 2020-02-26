#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/vulkan/Pipeline.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

#include <filesystem>
#include <fstream>

namespace caelus::core::vulkan {
    vk::ShaderModule load_module(const types::detail::VulkanContext* ctx, const std::filesystem::path& path) {
        std::ifstream in(path.generic_string(), std::fstream::binary);

        if (!in.is_open()) {
            throw std::runtime_error("Error, \"" + path.filename().generic_string() + "\" file not found.");
        }

        std::string spv{ std::istreambuf_iterator<char>{ in }, {} };

        vk::ShaderModuleCreateInfo create_info{}; {
            create_info.codeSize = spv.size();
            create_info.pCode = reinterpret_cast<const u32*>(spv.data());
        }

        auto module = ctx->device_details.device.createShaderModule(create_info, nullptr, ctx->dispatcher);

        logger::info("Module \"" + path.filename().generic_string() + "\" successfully loaded");

        return module;
    }


    Pipeline make_generic_mesh_pipeline(const types::info::PipelineCreateInfo& info) {
        Pipeline pipeline{};

        std::array<vk::ShaderModule, 2> modules{}; {
            modules[0] = load_module(info.ctx, info.vertex_path);
            modules[1] = load_module(info.ctx, info.fragment_path);
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

        std::array<vk::VertexInputBindingDescription, 1> vertex_bindings{}; {
            vertex_bindings[0].stride = sizeof(types::Vertex);
            vertex_bindings[0].binding = 0;
            vertex_bindings[0].inputRate = vk::VertexInputRate::eVertex;
        }

        std::array<vk::VertexInputAttributeDescription, 2> vertex_attributes{}; {
            vertex_attributes[0].binding = 0;
            vertex_attributes[0].format = vk::Format::eR32G32B32Sfloat;
            vertex_attributes[0].location = 0;
            vertex_attributes[0].offset = offsetof(types::Vertex, pos);

            vertex_attributes[1].binding = 0;
            vertex_attributes[1].format = vk::Format::eR32G32Sfloat;
            vertex_attributes[1].location = 1;
            vertex_attributes[1].offset = offsetof(types::Vertex, tx_coords);
        }

        vk::PipelineVertexInputStateCreateInfo vertex_input_info{}; {
            vertex_input_info.pVertexBindingDescriptions = vertex_bindings.data();
            vertex_input_info.vertexBindingDescriptionCount = vertex_bindings.size();
            vertex_input_info.pVertexAttributeDescriptions = vertex_attributes.data();
            vertex_input_info.vertexAttributeDescriptionCount = vertex_attributes.size();
        }

        vk::PipelineInputAssemblyStateCreateInfo input_assembly{}; {
            input_assembly.topology = vk::PrimitiveTopology::eTriangleList;
            input_assembly.primitiveRestartEnable = false;
        }

        vk::Viewport viewport{};
        vk::Rect2D scissor{};

        vk::PipelineViewportStateCreateInfo viewport_state{}; {
            viewport_state.viewportCount = 1;
            viewport_state.pViewports = &viewport;
            viewport_state.scissorCount = 1;
            viewport_state.pScissors = &scissor;
        }

        vk::PipelineRasterizationStateCreateInfo rasterizer_state_info{}; {
            rasterizer_state_info.lineWidth = 1.0f;
            rasterizer_state_info.depthBiasEnable = false;
            rasterizer_state_info.depthClampEnable = false;
            rasterizer_state_info.rasterizerDiscardEnable = false;
            rasterizer_state_info.polygonMode = vk::PolygonMode::eFill;
            rasterizer_state_info.cullMode = vk::CullModeFlagBits::eNone;
            rasterizer_state_info.frontFace = vk::FrontFace::eCounterClockwise;
        }

        vk::PipelineMultisampleStateCreateInfo multisampling_state_info{}; {
            multisampling_state_info.alphaToCoverageEnable = false;
            multisampling_state_info.sampleShadingEnable = false;
            multisampling_state_info.alphaToOneEnable = false;
            multisampling_state_info.rasterizationSamples = vk::SampleCountFlagBits::e1;
            multisampling_state_info.minSampleShading = 1.0f;
            multisampling_state_info.pSampleMask = nullptr;
        }

        vk::PipelineDepthStencilStateCreateInfo depth_stencil_info{}; {
            depth_stencil_info.stencilTestEnable = false;
            depth_stencil_info.depthTestEnable = true;
            depth_stencil_info.depthCompareOp = vk::CompareOp::eLess;
        }

        vk::PipelineColorBlendAttachmentState color_blend_attachment{}; {
            color_blend_attachment.blendEnable = false;
            color_blend_attachment.colorWriteMask =
                vk::ColorComponentFlagBits::eR |
                vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB |
                vk::ColorComponentFlagBits::eA;
            color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eOne;
            color_blend_attachment.dstColorBlendFactor = vk::BlendFactor::eZero;
            color_blend_attachment.colorBlendOp = vk::BlendOp::eAdd;
            color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
            color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
            color_blend_attachment.alphaBlendOp = vk::BlendOp::eAdd;
        }

        vk::PipelineColorBlendStateCreateInfo color_blend_info{}; {
            color_blend_info.attachmentCount = 1;
            color_blend_info.pAttachments = &color_blend_attachment;
            color_blend_info.logicOp = vk::LogicOp::eCopy;
            color_blend_info.logicOpEnable = false;
            color_blend_info.blendConstants[0] = 0.0f;
            color_blend_info.blendConstants[1] = 0.0f;
            color_blend_info.blendConstants[2] = 0.0f;
            color_blend_info.blendConstants[3] = 0.0f;
        }
       
        vk::PipelineLayoutCreateInfo layout_create_info{}; {
            layout_create_info.setLayoutCount = 0;
            layout_create_info.pSetLayouts = nullptr;
            layout_create_info.pushConstantRangeCount = 0;
            layout_create_info.pPushConstantRanges = nullptr;
        }

        pipeline.layout.pipeline_layout = info.ctx->device_details.device.createPipelineLayout(layout_create_info, nullptr, info.ctx->dispatcher);

        vk::GraphicsPipelineCreateInfo pipeline_info{}; {
            pipeline_info.stageCount = stages.size();
            pipeline_info.pStages = stages.data();
            pipeline_info.pVertexInputState = &vertex_input_info;
            pipeline_info.pInputAssemblyState = &input_assembly;
            pipeline_info.pViewportState = &viewport_state;
            pipeline_info.pRasterizationState = &rasterizer_state_info;
            pipeline_info.pMultisampleState = &multisampling_state_info;
            pipeline_info.pDepthStencilState = &depth_stencil_info;
            pipeline_info.pColorBlendState = &color_blend_info;
            pipeline_info.pDynamicState = &dynamic_state_create_info;
            pipeline_info.layout = pipeline.layout.pipeline_layout;
            pipeline_info.renderPass = info.ctx->render_passes[info.renderpass_index];
            pipeline_info.subpass = info.subpass_index;
            pipeline_info.basePipelineHandle = nullptr;
            pipeline_info.basePipelineIndex = -1;
        }

        pipeline.pipeline = info.ctx->device_details.device.createGraphicsPipeline(nullptr, pipeline_info, nullptr, info.ctx->dispatcher);

        logger::info("Generic pipeline successfully created");

        for (const auto& module : modules) {
            info.ctx->device_details.device.destroyShaderModule(module, nullptr, info.ctx->dispatcher);
        }

        return pipeline;
    }
} // namespace caelus::core::vulkan