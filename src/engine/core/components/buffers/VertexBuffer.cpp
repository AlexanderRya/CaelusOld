#include "engine/core/components/buffers/VertexBuffer.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/util/Util.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Types.hpp"

#include <cstring>

namespace caelus::core::components::buffers {
    VertexBuffer::VertexBuffer(const types::info::VertexBufferCreateInfo& info)
        : ctx(info.ctx) {
        vk::Buffer temp_buffer{};
        vk::DeviceMemory temp_memory{};

        /* Allocate staging buffer */ {
            temp_buffer = util::make_buffer(
                info.vertices.size() * sizeof(types::Vertex),
                vk::BufferUsageFlagBits::eTransferSrc,
                *info.ctx);

            temp_memory = util::allocate_memory(
                temp_buffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                *info.ctx);

            auto mapped = info.ctx->device_details.device.mapMemory(temp_memory, 0, info.vertices.size() * sizeof(types::Vertex), {}, info.ctx->dispatcher);
            std::memcpy(mapped, info.vertices.data(), info.vertices.size() * sizeof(types::Vertex));
            info.ctx->device_details.device.unmapMemory(temp_memory, info.ctx->dispatcher);
        }

        /* Allocate device local buffer */ {
            buffer = util::make_buffer(
                info.vertices.size() * sizeof(types::Vertex),
                vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                *info.ctx);

            memory = util::allocate_memory(buffer, vk::MemoryPropertyFlagBits::eDeviceLocal, *info.ctx);
        }

        /* Copy to device local */ {
            util::copy_buffer(temp_buffer, buffer, info.vertices.size() * sizeof(types::Vertex), *info.ctx);

            info.ctx->device_details.device.freeMemory(temp_memory, nullptr, info.ctx->dispatcher);
            info.ctx->device_details.device.destroyBuffer(temp_buffer, nullptr, info.ctx->dispatcher);
        }

        logger::info("Successfully allocated vertex buffer with size (in bytes): ", info.vertices.size() * sizeof(types::Vertex));
    }

    void VertexBuffer::deallocate() {
        ctx->device_details.device.unmapMemory(memory, ctx->dispatcher);

        ctx->device_details.device.freeMemory(memory, nullptr, ctx->dispatcher);
        ctx->device_details.device.destroyBuffer(buffer, nullptr, ctx->dispatcher);
    }
} // namespace caelus::core::components::buffers