#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/vulkan/DescriptorSet.hpp"
#include "engine/core/Constants.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::vulkan {
    void DescriptorSet::create(const types::detail::VulkanContext* context, const vk::DescriptorSetLayout& layout) {
        ctx = context;
        vk::DescriptorSetAllocateInfo allocate_info{}; {
            allocate_info.descriptorPool = ctx->descriptor_pool;
            allocate_info.descriptorSetCount = 1;
            allocate_info.pSetLayouts = &layout;
        }

        handles.reserve(constants::frames_in_flight);
        for (int i = 0; i < constants::frames_in_flight; ++i) {
            handles.emplace_back(ctx->device_details.device.allocateDescriptorSets(allocate_info, ctx->dispatcher).back());
        }
    }

    vk::DescriptorSet& DescriptorSet::operator [](const usize idx) {
        return handles[idx];
    }
} // namespace caelus::core::vulkan