#include "engine/core/components/buffers/InstanceBuffer.hpp"
#include "engine/core/renderer/vulkan/Device.hpp"
#include "engine/core/details/VulkanContext.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Types.hpp"

#include <cstring>

namespace caelus::core::components::buffers {
    void InstanceBuffer::allocate(const std::vector<glm::mat4>& instances, const types::detail::VulkanContext& ctx) {
        vk::Buffer temp_buffer{};
        vk::DeviceMemory temp_memory{};

        /* Allocate staging buffer */ {
            vk::BufferCreateInfo buffer_create_info{}; {
                buffer_create_info.size = instances.size() * sizeof(glm::mat4);
                buffer_create_info.queueFamilyIndexCount = 1;
                buffer_create_info.pQueueFamilyIndices = &ctx.device_details.queue_family;
                buffer_create_info.usage = vk::BufferUsageFlagBits::eTransferSrc;
                buffer_create_info.sharingMode = vk::SharingMode::eExclusive;
            }

            temp_buffer = ctx.device_details.device.createBuffer(buffer_create_info);

            const auto memory_requirements = ctx.device_details.device.getBufferMemoryRequirements(temp_buffer);

            vk::MemoryAllocateInfo memory_allocate_info{}; {
                memory_allocate_info.allocationSize = memory_requirements.size;
                memory_allocate_info.memoryTypeIndex = vulkan::find_memory_type(
                    ctx,
                    memory_requirements.memoryTypeBits,
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
            }

            temp_memory = ctx.device_details.device.allocateMemory(memory_allocate_info);

            ctx.device_details.device.bindBufferMemory(temp_buffer, temp_memory, 0);

            auto mapped = ctx.device_details.device.mapMemory(temp_memory, 0, instances.size() * sizeof(glm::mat4));
            std::memcpy(mapped, instances.data(), instances.size() * sizeof(glm::mat4));
            ctx.device_details.device.unmapMemory(temp_memory);
        }

        /* Allocate device local buffer */ {
            vk::BufferCreateInfo buffer_create_info{}; {
                buffer_create_info.size = instances.size() * sizeof(glm::mat4);
                buffer_create_info.queueFamilyIndexCount = 1;
                buffer_create_info.pQueueFamilyIndices = &ctx.device_details.queue_family;
                buffer_create_info.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
                buffer_create_info.sharingMode = vk::SharingMode::eExclusive;
            }

            buffer = ctx.device_details.device.createBuffer(buffer_create_info);

            const auto memory_requirements = ctx.device_details.device.getBufferMemoryRequirements(buffer);

            vk::MemoryAllocateInfo memory_allocate_info{}; {
                memory_allocate_info.allocationSize = memory_requirements.size;
                memory_allocate_info.memoryTypeIndex = vulkan::find_memory_type(
                    ctx,
                    memory_requirements.memoryTypeBits,
                    vk::MemoryPropertyFlagBits::eDeviceLocal);
            }

            memory = ctx.device_details.device.allocateMemory(memory_allocate_info);

            ctx.device_details.device.bindBufferMemory(buffer, memory, 0);
        }

        /* Copy to device local */ {
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
                region.size = instances.size() * sizeof(glm::mat4);
                region.srcOffset = 0;
                region.dstOffset = 0;
            }

            temp_command_buffer[0].copyBuffer(temp_buffer, buffer, region);

            temp_command_buffer[0].end();

            vk::SubmitInfo submit_info{}; {
                submit_info.commandBufferCount = 1;
                submit_info.pCommandBuffers = temp_command_buffer.data();
            }

            ctx.device_details.queue.submit(submit_info, nullptr);

            ctx.device_details.queue.waitIdle();

            ctx.device_details.device.freeCommandBuffers(ctx.transient_pool, temp_command_buffer);

            ctx.device_details.device.freeMemory(temp_memory);
            ctx.device_details.device.destroyBuffer(temp_buffer);
        }

        logger::info("Successfully allocated instance buffer with size (in bytes): ", instances.size() * sizeof(glm::mat4));
    }

    void InstanceBuffer::deallocate(const types::detail::VulkanContext& ctx) {
        ctx.device_details.device.freeMemory(memory);
        ctx.device_details.device.destroyBuffer(buffer);
    }
} // namespace caelus::core::components::buffers