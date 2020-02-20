#include "engine/core/components/manager/PipelineLayoutManager.hpp"
#include "engine/core/components/buffers/DescriptorBuffer.hpp"
#include "engine/core/components/manager/ResourceManager.hpp"
#include "engine/core/renderer/vulkan/DescriptorSet.hpp"
#include "engine/core/details/VulkanContext.hpp"
#include "engine/core/Constants.hpp"
#include "engine/core/info/Info.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::vulkan {
    void DescriptorSet::create(const types::info::DescriptorSetInfo& info) {
        details.buffer_id = info.buffer_id;
        details.type = info.type;
        details.binding = info.binding;

        vk::DescriptorSetAllocateInfo allocate_info{}; {
            allocate_info.pSetLayouts = &manager::PipelineLayoutManager::get_layout(info.layout_id).set_layout;
            allocate_info.descriptorSetCount = 1;
            allocate_info.descriptorPool = info.ctx->descriptor_pool;
        }

        details.descriptor_set = info.ctx->device_details.device.allocateDescriptorSets(allocate_info).back();
    }

    void DescriptorSet::bind_buffer(const u32 idx, const types::detail::VulkanContext& ctx) {
        details.buffer_idx = idx; // Current active buffer index

        auto& buffer = manager::ResourceManager::get_descriptor_buffers(details.buffer_id)[idx];

        vk::DescriptorBufferInfo buffer_info{}; {
            buffer_info.range = buffer.current_size;
            buffer_info.offset = 0;
            buffer_info.buffer = buffer.buffer;
        }

        vk::WriteDescriptorSet write_descriptor_set{}; {
            write_descriptor_set.dstBinding = details.binding;
            write_descriptor_set.pBufferInfo = &buffer_info;
            write_descriptor_set.descriptorType = details.type;
            write_descriptor_set.descriptorCount = 1;
            write_descriptor_set.dstArrayElement = 0;
            write_descriptor_set.dstSet = details.descriptor_set;
        }

        ctx.device_details.device.updateDescriptorSets(write_descriptor_set, nullptr);
    }

    void DescriptorSet::update(const types::detail::VulkanContext& ctx) {
        auto& buffer = manager::ResourceManager::get_descriptor_buffers(details.buffer_id)[details.buffer_idx];

        vk::DescriptorBufferInfo buffer_info{}; {
            buffer_info.range = buffer.current_size;
            buffer_info.offset = 0;
            buffer_info.buffer = buffer.buffer;
        }

        vk::WriteDescriptorSet write_descriptor_set{}; {
            write_descriptor_set.dstBinding = details.binding;
            write_descriptor_set.pBufferInfo = &buffer_info;
            write_descriptor_set.descriptorType = details.type;
            write_descriptor_set.descriptorCount = 1;
            write_descriptor_set.dstArrayElement = 0;
            write_descriptor_set.dstSet = details.descriptor_set;
        }

        ctx.device_details.device.updateDescriptorSets(write_descriptor_set, nullptr);
    }

    [[nodiscard]] std::vector<DescriptorSet> make_descriptor_sets(const types::info::DescriptorSetInfo& info) {
        std::vector<DescriptorSet> sets(constants::max_frames_in_flight);

        for (usize i = 0; i < sets.size(); ++i) {
            sets[i].create(info);
            sets[i].bind_buffer(i, *info.ctx);
        }

        logger::info("Descriptor set successfully created");

        return sets;
    }
}