#include "engine/core/components/buffers/InstanceBuffer.hpp"
#include "engine/core/renderer/vulkan/Device.hpp"
#include "engine/core/details/VulkanContext.hpp"
#include "engine/core/util/Util.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Types.hpp"

#include <cstring>

namespace caelus::core::components::buffers {
    void InstanceBuffer::allocate(const std::vector<glm::mat4>& instances, const types::detail::VulkanContext& ctx) {
        vk::Buffer temp_buffer{};
        vk::DeviceMemory temp_memory{};

        /* Allocate staging buffer */ {
            temp_buffer = util::make_buffer(
                instances.size() * sizeof(glm::mat4),
                vk::BufferUsageFlagBits::eTransferSrc,
                ctx);

            temp_memory = util::allocate_memory(
                temp_buffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                ctx);

            auto mapped = ctx.device_details.device.mapMemory(temp_memory, 0, instances.size() * sizeof(glm::mat4));
            std::memcpy(mapped, instances.data(), instances.size() * sizeof(glm::mat4));
            ctx.device_details.device.unmapMemory(temp_memory);
        }

        /* Allocate device local buffer */ {
            buffer = util::make_buffer(
                instances.size() * sizeof(glm::mat4),
                vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                ctx);

            memory = util::allocate_memory(
                buffer,
                vk::MemoryPropertyFlagBits::eDeviceLocal,
                ctx);
        }

        /* Copy to device local */ {
            util::copy_buffer(temp_buffer, buffer, instances.size() * sizeof(glm::mat4), ctx);

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