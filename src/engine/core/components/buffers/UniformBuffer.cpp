#include "engine/core/components/buffers/UniformBuffer.hpp"
#include "engine/core/renderer/vulkan/Device.hpp"
#include "engine/core/details/VulkanContext.hpp"
#include "engine/core/Types.hpp"
#include <cstring>

namespace caelus::core::components::buffers {
    void UniformBuffer::allocate(const types::detail::VulkanContext& ctx) {
        vk::BufferCreateInfo buffer_create_info{}; {
            buffer_create_info.size = sizeof(types::TransformUBO);
            buffer_create_info.queueFamilyIndexCount = 1;
            buffer_create_info.pQueueFamilyIndices = &ctx.device_details.queue_family;
            buffer_create_info.usage = vk::BufferUsageFlagBits::eUniformBuffer;
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

        mapped = ctx.device_details.device.mapMemory(memory, 0, sizeof(types::TransformUBO));
    }

    void UniformBuffer::write(const types::TransformUBO& data) {
        std::memcpy(mapped, &data, sizeof(types::TransformUBO));
    }

    void UniformBuffer::deallocate(const types::detail::VulkanContext& ctx) {
        ctx.device_details.device.unmapMemory(memory);

        ctx.device_details.device.freeMemory(memory);
        ctx.device_details.device.destroyBuffer(buffer);
    }
} // namespace caelus::core::components::buffers