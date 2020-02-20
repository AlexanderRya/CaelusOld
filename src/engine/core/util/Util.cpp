#include "engine/core/renderer/vulkan/Device.hpp"
#include "engine/core/details/VulkanContext.hpp"
#include "engine/core/util/Util.hpp"
#include "vulkan/vulkan.hpp"

namespace caelus::core::util {
    vk::Buffer make_buffer(const usize size, const vk::BufferUsageFlags& usage, const types::detail::VulkanContext& ctx) {
        vk::BufferCreateInfo buffer_create_info{}; {
            buffer_create_info.size = size;
            buffer_create_info.queueFamilyIndexCount = 1;
            buffer_create_info.pQueueFamilyIndices = &ctx.device_details.queue_family;
            buffer_create_info.usage = usage;
            buffer_create_info.sharingMode = vk::SharingMode::eExclusive;
        }

        return ctx.device_details.device.createBuffer(buffer_create_info);
    }

    vk::DeviceMemory allocate_memory(const vk::Buffer& buffer, const vk::MemoryPropertyFlags& flags, const types::detail::VulkanContext& ctx) {
        const auto memory_requirements = ctx.device_details.device.getBufferMemoryRequirements(buffer);

        vk::MemoryAllocateInfo memory_allocate_info{}; {
            memory_allocate_info.allocationSize = memory_requirements.size;
            memory_allocate_info.memoryTypeIndex = vulkan::find_memory_type(
                ctx, memory_requirements.memoryTypeBits, flags);
        }

        auto memory = ctx.device_details.device.allocateMemory(memory_allocate_info);

        ctx.device_details.device.bindBufferMemory(buffer, memory, 0);

        return memory;
    }

    void copy_buffer(const vk::Buffer& src, const vk::Buffer& dst, const usize size, const types::detail::VulkanContext& ctx) {
        vk::CommandBufferAllocateInfo command_buffer_allocate_info{}; {
            command_buffer_allocate_info.commandBufferCount = 1;
            command_buffer_allocate_info.level = vk::CommandBufferLevel::ePrimary;
            command_buffer_allocate_info.commandPool = ctx.transient_pool;
        }

        auto temp_command_buffer = ctx.device_details.device.allocateCommandBuffers(command_buffer_allocate_info);

        vk::CommandBufferBeginInfo begin_info{}; {
            begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        }

        temp_command_buffer[0].begin(begin_info);

        vk::BufferCopy region{}; {
            region.size = size;
            region.srcOffset = 0;
            region.dstOffset = 0;
        }

        temp_command_buffer[0].copyBuffer(src, dst, region);

        temp_command_buffer[0].end();

        vk::SubmitInfo submit_info{}; {
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = temp_command_buffer.data();
        }

        ctx.device_details.queue.submit(submit_info, nullptr);

        ctx.device_details.queue.waitIdle();

        ctx.device_details.device.freeCommandBuffers(ctx.transient_pool, temp_command_buffer);
    }
} // namespace caelus::core::util