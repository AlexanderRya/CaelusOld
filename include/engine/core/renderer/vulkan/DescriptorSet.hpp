#ifndef CAELUS_DESCRIPTORSET_HPP
#define CAELUS_DESCRIPTORSET_HPP

#include "engine/core/components/manager/DescriptorLayoutManager.hpp"
#include "engine/core/info/DescriptorSetCreateInfo.hpp"
#include "engine/core/renderer/vulkan/Device.hpp"
#include "engine/core/details/VulkanContext.hpp"
#include "engine/core/Constants.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::vulkan {
    template <typename Ty>
    struct DescriptorSet {
        vk::Buffer buffer;
        vk::DeviceMemory memory;
        vk::DescriptorSet descriptor_set;

        vk::DescriptorType descriptor_type{};
        vk::BufferUsageFlags usage_flags{};
        u32 binding{};

        void* mapped{};
        usize size{};

        DescriptorSet() = default;

        void create(const types::info::DescriptorSetCreateInfo&);
        void write(const std::vector<Ty>&, const types::detail::VulkanContext&);
        void deallocate(const types::detail::VulkanContext&);
    };

    template <typename Ty>
    void DescriptorSet<Ty>::create(const types::info::DescriptorSetCreateInfo& info) {
        vk::BufferCreateInfo buffer_create_info{}; {
            buffer_create_info.size = sizeof(Ty);
            buffer_create_info.queueFamilyIndexCount = 1;
            buffer_create_info.pQueueFamilyIndices = &info.ctx->device_details.queue_family;
            buffer_create_info.usage = info.usage_flags;
            buffer_create_info.sharingMode = vk::SharingMode::eExclusive;
        }

        usage_flags = info.usage_flags;
        buffer = info.ctx->device_details.device.createBuffer(buffer_create_info);

        const auto memory_requirements = info.ctx->device_details.device.getBufferMemoryRequirements(buffer);

        vk::MemoryAllocateInfo memory_allocate_info{}; {
            memory_allocate_info.allocationSize = memory_requirements.size;
            memory_allocate_info.memoryTypeIndex = vulkan::find_memory_type(
                *info.ctx,
                memory_requirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        }

        memory = info.ctx->device_details.device.allocateMemory(memory_allocate_info);

        info.ctx->device_details.device.bindBufferMemory(buffer, memory, 0);
        this->size = 1;

        mapped = info.ctx->device_details.device.mapMemory(memory, 0, sizeof(Ty));

        vk::DescriptorSetAllocateInfo allocate_info{}; {
            allocate_info.descriptorSetCount = 1;
            allocate_info.pSetLayouts = &manager::DescriptorLayoutManager::get_layout(info.layout_id);
            allocate_info.descriptorPool = info.ctx->descriptor_pool;
        }

        descriptor_set = info.ctx->device_details.device.allocateDescriptorSets(allocate_info)[0];

        // Write initial data
        vk::DescriptorBufferInfo buffer_info{}; {
            buffer_info.buffer = buffer;
            buffer_info.offset = 0;
            buffer_info.range = sizeof(Ty);
        }

        vk::WriteDescriptorSet write{}; {
            write.descriptorType = info.descriptor_type;
            write.dstBinding = info.binding;
            write.pBufferInfo = &buffer_info;
            write.dstArrayElement = 0;
            write.descriptorCount = 1;
            write.dstSet = descriptor_set;
        }

        descriptor_type = info.descriptor_type;
        binding = info.binding;

        info.ctx->device_details.device.updateDescriptorSets(write, nullptr);
    }

    template <typename Ty>
    void DescriptorSet<Ty>::write(const std::vector<Ty>& data, const types::detail::VulkanContext& ctx) {
        if (data.empty()) {
            return;
        }

        if (data.size() <= size) {
            std::memcpy(mapped, data.data(), data.size() * sizeof(Ty));
            return;
        } else {
            logger::info("Resizing descriptor set buffer with new_size: ", data.size());

            deallocate(ctx);

            vk::BufferCreateInfo buffer_create_info{}; {
                buffer_create_info.size = data.size() * sizeof(Ty);
                buffer_create_info.queueFamilyIndexCount = 1;
                buffer_create_info.pQueueFamilyIndices = &ctx.device_details.queue_family;
                buffer_create_info.usage = usage_flags;
                buffer_create_info.sharingMode = vk::SharingMode::eExclusive;
            }

            buffer = ctx.device_details.device.createBuffer(buffer_create_info);

            const auto memory_requirements = ctx.device_details.device.getBufferMemoryRequirements(buffer);

            vk::MemoryAllocateInfo memory_allocate_info{}; {
                memory_allocate_info.allocationSize = memory_requirements.size;
                memory_allocate_info.memoryTypeIndex = vulkan::find_memory_type(
                    ctx,
                    memory_requirements.memoryTypeBits,
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
            }

            memory = ctx.device_details.device.allocateMemory(memory_allocate_info);

            ctx.device_details.device.bindBufferMemory(buffer, memory, 0);
            this->size = data.size();

            mapped = ctx.device_details.device.mapMemory(memory, 0, data.size() * sizeof(Ty));

            vk::DescriptorBufferInfo buffer_info{}; {
                buffer_info.buffer = buffer;
                buffer_info.offset = 0;
                buffer_info.range = data.size() * sizeof(Ty);
            }

            vk::WriteDescriptorSet write{}; {
                write.descriptorType = descriptor_type;
                write.dstBinding = binding;
                write.pBufferInfo = &buffer_info;
                write.dstArrayElement = 0;
                write.descriptorCount = 1;
                write.dstSet = descriptor_set;
            }

            ctx.device_details.device.updateDescriptorSets(write, nullptr);
        }
    }

    template <typename Ty>
    void DescriptorSet<Ty>::deallocate(const types::detail::VulkanContext& ctx) {
        ctx.device_details.device.unmapMemory(memory);

        ctx.device_details.device.freeMemory(memory);
        ctx.device_details.device.destroyBuffer(buffer);
    }
} // namespace caelus::core::vulkan

#endif //CAELUS_DESCRIPTORSET_HPP
