#include "engine/core/vulkan/PipelineLayout.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"

namespace caelus::core::vulkan {
    PipelineLayout create_pipeline_layout(const types::detail::VulkanContext& ctx, const PipelineLayoutType type) {
        PipelineLayout layout;

        if (type == PipelineLayoutType::GenericMesh) {
            std::array<vk::DescriptorSetLayoutBinding, 2> layout_bindings{}; {
                layout_bindings[0].descriptorCount = 1;
                layout_bindings[0].descriptorType = vk::DescriptorType::eUniformBuffer;
                layout_bindings[0].binding = 0;
                layout_bindings[0].stageFlags = vk::ShaderStageFlagBits::eVertex;

                layout_bindings[1].descriptorCount = 1;
                layout_bindings[1].descriptorType = vk::DescriptorType::eStorageBuffer;
                layout_bindings[1].binding = 1;
                layout_bindings[1].stageFlags = vk::ShaderStageFlagBits::eVertex;
            }

            vk::DescriptorSetLayoutCreateInfo set_layout_create_info{}; {
                set_layout_create_info.bindingCount = layout_bindings.size();
                set_layout_create_info.pBindings = layout_bindings.data();
            }

            layout.set_layout = ctx.device_details.device.createDescriptorSetLayout(set_layout_create_info, nullptr, ctx.dispatcher);

            vk::PipelineLayoutCreateInfo layout_create_info{}; {
                layout_create_info.setLayoutCount = 1;
                layout_create_info.pSetLayouts = &layout.set_layout;
                layout_create_info.pushConstantRangeCount = 0;
                layout_create_info.pPushConstantRanges = nullptr;
            }

            layout.pipeline_layout = ctx.device_details.device.createPipelineLayout(layout_create_info, nullptr, ctx.dispatcher);

            return layout;
        }

        throw std::runtime_error("Nope, not yet implemented");
    }
} // namespace caelus::core::vulkan