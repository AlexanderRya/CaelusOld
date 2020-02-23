#include "engine/core/components/buffers/VertexBuffer.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/util/Util.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Types.hpp"

#include <cstring>

namespace caelus::core::components::buffers {
    void VertexBuffer::allocate(std::vector<types::Vertex>&& vertices) {
        vk::Buffer temp_buffer{};
        vk::DeviceMemory temp_memory{};

        /* Allocate staging buffer */ {
            temp_buffer = util::make_buffer(
                vertices.size() * sizeof(types::Vertex),
                vk::BufferUsageFlagBits::eTransferSrc,
                ctx);

            temp_memory = util::allocate_memory(
                temp_buffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                ctx);

            auto mapped = ctx.device_details.device.mapMemory(temp_memory, 0, vertices.size() * sizeof(types::Vertex));
            std::memcpy(mapped, vertices.data(), vertices.size() * sizeof(types::Vertex));
            ctx.device_details.device.unmapMemory(temp_memory);
        }

        /* Allocate device local buffer */ {
            buffer = util::make_buffer(
                vertices.size() * sizeof(types::Vertex),
                vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                ctx);

            memory = util::allocate_memory(buffer, vk::MemoryPropertyFlagBits::eDeviceLocal, ctx);
        }

        /* Copy to device local */ {
            util::copy_buffer(temp_buffer, buffer, vertices.size() * sizeof(types::Vertex), ctx);

            ctx.device_details.device.freeMemory(temp_memory);
            ctx.device_details.device.destroyBuffer(temp_buffer);
        }

        logger::info("Successfully allocated vertex buffer with size (in bytes): ", vertices.size() * sizeof(types::Vertex));
    }

    void VertexBuffer::deallocate() {
        ctx.device_details.device.freeMemory(memory);
        ctx.device_details.device.destroyBuffer(buffer);
    }
} // namespace caelus::core::components::buffers