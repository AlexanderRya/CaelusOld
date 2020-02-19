#include "engine/core/components/buffers/InstanceBuffer.hpp"
#include "engine/core/renderer/vulkan/Device.hpp"
#include "engine/core/details/VulkanData.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Types.hpp"

#include <cstring>

namespace caelus::core::components::buffers {
    void InstanceBuffer::allocate(const std::vector<glm::mat4>& instances, const types::detail::VulkanData& data) {
        vk::Buffer temp_buffer{};
        vk::DeviceMemory temp_memory{};

        /* Allocate staging buffer */ {
            vk::BufferCreateInfo buffer_create_info{}; {
                buffer_create_info.size = instances.size() * sizeof(glm::mat4);
                buffer_create_info.queueFamilyIndexCount = 1;
                buffer_create_info.pQueueFamilyIndices = &data.device_details.queue_family;
                buffer_create_info.usage = vk::BufferUsageFlagBits::eTransferSrc;
                buffer_create_info.sharingMode = vk::SharingMode::eExclusive;
            }

            temp_buffer = data.device_details.device.createBuffer(buffer_create_info);

            const auto memory_requirements = data.device_details.device.getBufferMemoryRequirements(temp_buffer);

            vk::MemoryAllocateInfo memory_allocate_info{}; {
                memory_allocate_info.allocationSize = memory_requirements.size;
                memory_allocate_info.memoryTypeIndex = vulkan::find_memory_type(
                    data,
                    memory_requirements.memoryTypeBits,
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
            }

            temp_memory = data.device_details.device.allocateMemory(memory_allocate_info);

            data.device_details.device.bindBufferMemory(temp_buffer, temp_memory, 0);

            auto mapped = data.device_details.device.mapMemory(temp_memory, 0, instances.size() * sizeof(glm::mat4));
            std::memcpy(mapped, instances.data(), instances.size() * sizeof(glm::mat4));
            data.device_details.device.unmapMemory(temp_memory);
        }

        /* Allocate device local buffer */ {
            vk::BufferCreateInfo buffer_create_info{}; {
                buffer_create_info.size = instances.size() * sizeof(glm::mat4);
                buffer_create_info.queueFamilyIndexCount = 1;
                buffer_create_info.pQueueFamilyIndices = &data.device_details.queue_family;
                buffer_create_info.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
                buffer_create_info.sharingMode = vk::SharingMode::eExclusive;
            }

            buffer = data.device_details.device.createBuffer(buffer_create_info);

            const auto memory_requirements = data.device_details.device.getBufferMemoryRequirements(buffer);

            vk::MemoryAllocateInfo memory_allocate_info{}; {
                memory_allocate_info.allocationSize = memory_requirements.size;
                memory_allocate_info.memoryTypeIndex = vulkan::find_memory_type(
                    data,
                    memory_requirements.memoryTypeBits,
                    vk::MemoryPropertyFlagBits::eDeviceLocal);
            }

            memory = data.device_details.device.allocateMemory(memory_allocate_info);

            data.device_details.device.bindBufferMemory(buffer, memory, 0);
        }

        /* Copy to device local */ {
            vk::CommandBufferAllocateInfo command_buffer_allocate_info{}; {
                command_buffer_allocate_info.commandBufferCount = 1;
                command_buffer_allocate_info.level = vk::CommandBufferLevel::ePrimary;
                command_buffer_allocate_info.commandPool = data.transient_pool;
            }

            auto temp_command_buffer = data.device_details.device.allocateCommandBuffers(command_buffer_allocate_info);

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

            data.device_details.queue.submit(submit_info, nullptr);

            data.device_details.queue.waitIdle();

            data.device_details.device.freeCommandBuffers(data.transient_pool, temp_command_buffer);

            data.device_details.device.freeMemory(temp_memory);
            data.device_details.device.destroyBuffer(temp_buffer);
        }

        logger::info("Successfully allocated instance buffer with size (in bytes): ", instances.size() * sizeof(glm::mat4));
    }
} // namespace caelus::core::components::buffers