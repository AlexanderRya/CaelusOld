#include "engine/core/renderer/vulkan/Pipeline.hpp"
#include "engine/core/info/PipelineCreateInfo.hpp"
#include "engine/logger/Logger.hpp"
#include "vulkan/vulkan.hpp"

#include <filesystem>
#include <fstream>

namespace caelus::engine::core::vulkan {
    static vk::ShaderModule load_module(const vk::Device& device, const std::filesystem::path& path) {
        std::ifstream in(path.generic_string());

        if (!in.is_open()) {
            throw std::runtime_error("Error, \"" + path.filename().generic_string() + "\" file not found.");
        }

        std::string spv{ std::istreambuf_iterator<char>{ in }, {} };

        vk::ShaderModuleCreateInfo create_info{}; {
            create_info.codeSize = spv.size();
            create_info.pCode = reinterpret_cast<const u32*>(spv.data());
        }

        auto module = device.createShaderModule(create_info);

        logger::info("Module: \"" + path.filename().generic_string() + "\" successfully loaded");

        return module;
    }

    void Pipeline::create(const types::info::PipelineCreateInfo& info) {
        std::array<vk::ShaderModule, 2> modules{}; {
            modules[0] = load_module(info.device, info.vertex_path);
            modules[1] = load_module(info.device, info.fragment_path);
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

        vk::GraphicsPipelineCreateInfo pipeline_info{}; {
            pipeline_info.stageCount = stages.size();
            pipeline_info.pStages = stages.data();
            pipeline_info.pVertexInputState = &info.vertex_input_info;
            pipeline_info.pInputAssemblyState = &info.input_assembly;
            pipeline_info.pViewportState = &info.viewport_state_info;
            pipeline_info.pRasterizationState = &info.rasterizer_state_info;
            pipeline_info.pMultisampleState = &info.multisampling_state_info;
            pipeline_info.pDepthStencilState = &info.depth_stencil_info;
            pipeline_info.pColorBlendState = &info.color_blend_info;
            pipeline_info.pDynamicState = nullptr;
            pipeline_info.layout = info.pipeline_layout;
            pipeline_info.renderPass = info.render_pass;
            pipeline_info.subpass = info.subpass_index;
            pipeline_info.basePipelineHandle = nullptr;
            pipeline_info.basePipelineIndex = -1;
        }

        auto graphics_pipeline = info.device.createGraphicsPipeline(nullptr, pipeline_info);

        logger::info("Pipeline successfully created");

        pipeline = graphics_pipeline;

        for (const auto& module : modules) {
            info.device.destroyShaderModule(module);
        }
    }
} // namespace caelus::engine::core::vulkan