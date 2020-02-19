#include "engine/core/info/PipelineCreateInfo.hpp"

namespace caelus::core::types::info {
    PipelineCreateInfo::PipelineCreateInfo() {
        static std::array<vk::VertexInputBindingDescription, 2> vertex_bindings{}; {
            vertex_bindings[0].stride = sizeof(Vertex);
            vertex_bindings[0].binding = 0;
            vertex_bindings[0].inputRate = vk::VertexInputRate::eVertex;

            vertex_bindings[1].stride = sizeof(glm::mat4);
            vertex_bindings[1].binding = 1;
            vertex_bindings[1].inputRate = vk::VertexInputRate::eInstance;
        }

        static std::array<vk::VertexInputAttributeDescription, 6> vertex_attributes{}; {
            vertex_attributes[0].binding = 0;
            vertex_attributes[0].format = vk::Format::eR32G32B32Sfloat;
            vertex_attributes[0].location = 0;
            vertex_attributes[0].offset = offsetof(Vertex, pos);

            vertex_attributes[1].binding = 0;
            vertex_attributes[1].format = vk::Format::eR32G32Sfloat;
            vertex_attributes[1].location = 1;
            vertex_attributes[1].offset = offsetof(Vertex, tx_coords);

            vertex_attributes[2].binding = 1;
            vertex_attributes[2].format = vk::Format::eR32G32B32A32Sfloat;
            vertex_attributes[2].location = 6;
            vertex_attributes[2].offset = 0;
            
            vertex_attributes[3].binding = 1;
            vertex_attributes[3].format = vk::Format::eR32G32B32A32Sfloat;
            vertex_attributes[3].location = 7;
            vertex_attributes[3].offset = sizeof(glm::vec4);

            vertex_attributes[4].binding = 1;
            vertex_attributes[4].format = vk::Format::eR32G32B32A32Sfloat;
            vertex_attributes[4].location = 8;
            vertex_attributes[4].offset = 2 * sizeof(glm::vec4);

            vertex_attributes[5].binding = 1;
            vertex_attributes[5].format = vk::Format::eR32G32B32A32Sfloat;
            vertex_attributes[5].location = 9;
            vertex_attributes[5].offset = 3 * sizeof(glm::vec4);
        }

        vertex_input_info.pVertexBindingDescriptions = vertex_bindings.data();
        vertex_input_info.vertexBindingDescriptionCount = vertex_bindings.size();
        vertex_input_info.pVertexAttributeDescriptions = vertex_attributes.data();
        vertex_input_info.vertexAttributeDescriptionCount = vertex_attributes.size();

        input_assembly.topology = vk::PrimitiveTopology::eTriangleList;
        input_assembly.primitiveRestartEnable = false;

        vk::Viewport viewport{};
        vk::Rect2D scissor{};

        viewport_state.viewportCount = 1;
        viewport_state.pViewports = &viewport;
        viewport_state.scissorCount = 1;
        viewport_state.pScissors = &scissor;

        rasterizer_state_info.lineWidth = 1.0f;
        rasterizer_state_info.depthBiasEnable = false;
        rasterizer_state_info.depthClampEnable = false;
        rasterizer_state_info.rasterizerDiscardEnable = false;
        rasterizer_state_info.polygonMode = vk::PolygonMode::eFill;
        rasterizer_state_info.cullMode = vk::CullModeFlagBits::eNone;
        rasterizer_state_info.frontFace = vk::FrontFace::eCounterClockwise;

        multisampling_state_info.alphaToCoverageEnable = false;
        multisampling_state_info.sampleShadingEnable = false;
        multisampling_state_info.alphaToOneEnable = false;
        multisampling_state_info.rasterizationSamples = vk::SampleCountFlagBits::e1;
        multisampling_state_info.minSampleShading = 1.0f;
        multisampling_state_info.pSampleMask = nullptr;

        depth_stencil_info.stencilTestEnable = false;
        depth_stencil_info.depthTestEnable = true;
        depth_stencil_info.depthCompareOp = vk::CompareOp::eLess;

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

        color_blend_info.attachmentCount = 1;
        color_blend_info.pAttachments = &color_blend_attachment;
        color_blend_info.logicOp = vk::LogicOp::eCopy;
        color_blend_info.logicOpEnable = false;
        color_blend_info.blendConstants[0] = 0.0f;
        color_blend_info.blendConstants[1] = 0.0f;
        color_blend_info.blendConstants[2] = 0.0f;
        color_blend_info.blendConstants[3] = 0.0f;
    }
} // namespace caelus::core::types::info