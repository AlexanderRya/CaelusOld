#include "engine/core/components/manager/DescriptorLayoutManager.hpp"
#include "engine/core/renderer/vulkan/DescriptorSet.hpp"
#include "engine/core/details/VulkanContext.hpp"

namespace caelus::core::vulkan {
    void DescriptorSet::create(const types::info::DescriptorSetCreateInfo& info) {

        vk::DescriptorSetAllocateInfo allocate_info{}; {
            allocate_info.pSetLayouts = &manager::DescriptorLayoutManager::get_layout(info.layout_id);
            allocate_info.descriptorPool = info.ctx->descriptor_pool;
            allocate_info.descriptorSetCount = 1;
        }

        descriptor_set = info.ctx->device_details.device.allocateDescriptorSets(allocate_info)[0];

        vk::DescriptorBufferInfo buffer_info{}; {
            buffer_info.offset = 0;
        }

        vk::WriteDescriptorSet write_descriptor_set{}; {
            write_descriptor_set.pBufferInfo = &buffer_info;
            write_descriptor_set.descriptorCount = 1;
            write_descriptor_set.dstBinding = info.binding;
            write_descriptor_set.dstArrayElement = 0;
            write_descriptor_set.dstSet = descriptor_set;
        }

        switch (info.type) {
            case experimental::components::buffers::BufferType::ActiveType::UniformBuffer: {
                buffer.uniform_buffer.allocate(*info.ctx);

                buffer_info.range = sizeof(types::TransformUBO);
                buffer_info.buffer = buffer.uniform_buffer.buffer;

                write_descriptor_set.descriptorType = vk::DescriptorType::eUniformBuffer;
            } break;
        }

        info.ctx->device_details.device.updateDescriptorSets(write_descriptor_set, nullptr);
    }

    std::array<DescriptorSet, constants::max_frames_in_flight> create_sets(const types::info::DescriptorSetCreateInfo& info) {
        std::array<DescriptorSet, constants::max_frames_in_flight> sets{};

        for (auto& each : sets) {
            each.create(info);
        }

        return sets;
    }
} // namespace caelus::core::vulkan