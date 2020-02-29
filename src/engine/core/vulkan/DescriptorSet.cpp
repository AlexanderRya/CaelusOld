#include "engine/core/vulkan/DescriptorSet.hpp"
#include "engine/core/Constants.hpp"

namespace caelus::core::vulkan {
    void SingleDescriptorSet::create(
        const types::detail::VulkanContext* context,
        const vk::DescriptorSetLayout& set_layout,
        const vk::DescriptorType set_type,
        const DescriptorBinding& set_binding) {

        ctx = context;
        layout = set_layout;
        type = set_type;
        binding = set_binding;

        vk::DescriptorSetAllocateInfo allocate_info{}; {
            allocate_info.descriptorPool = ctx->descriptor_pool;
            allocate_info.descriptorSetCount = 1;
            allocate_info.pSetLayouts = &layout;
        }

        descriptor = ctx->device_details.device.allocateDescriptorSets(allocate_info, ctx->dispatcher).back();
    }

    void SingleDescriptorSet::update(const vk::DescriptorBufferInfo& buffer_info) {
        vk::WriteDescriptorSet write{}; {
            write.descriptorType = type;
            write.descriptorCount = 1;
            write.dstArrayElement = 0;
            write.dstBinding = static_cast<u32>(binding);
            write.dstSet = descriptor;
            write.pBufferInfo = &buffer_info;
        }

        ctx->device_details.device.updateDescriptorSets(write, nullptr, ctx->dispatcher);
    }

    vk::DescriptorSet SingleDescriptorSet::handle() const {
        return descriptor;
    }

    DescriptorSet create_descriptor_set(
        const types::detail::VulkanContext* context,
        const vk::DescriptorSetLayout& set_layout,
        const vk::DescriptorType set_type,
        const DescriptorBinding& set_binding) {

        DescriptorSet sets{};

        for (int i = 0; i < constants::frames_in_flight; ++i) {
            sets.emplace_back().create(context, set_layout, set_type, set_binding);
        }

        return sets;
    }

    void update_descriptors(DescriptorSet& descriptors, const std::vector<vk::DescriptorBufferInfo>& infos) {
        for (usize i = 0; i < descriptors.size(); ++i) {
            descriptors[i].update(infos[i]);
        }
    }
} // namespace caelus::core::vulkan