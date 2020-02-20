#include "engine/core/components/buffers/DescriptorBuffer.hpp"
#include "engine/core/details/VulkanContext.hpp"
#include "engine/core/util/Util.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::components::buffers {
    void DescriptorBuffer::allocate(const usize size, const vk::BufferUsageFlags& flags, const types::detail::VulkanContext& ctx) {
        current_size = size;
        usage_flags = flags;

        buffer = util::make_buffer(size, flags, ctx);
        memory = util::allocate_memory(buffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, ctx);

        mapped = ctx.device_details.device.mapMemory(memory, 0, size);
    }

    void DescriptorBuffer::write(const void* data, const usize data_size, const types::detail::VulkanContext& ctx) {
        if (current_size == data_size) {
            std::memcpy(mapped, data, data_size);
        } else {
            deallocate(ctx);
            allocate(data_size, usage_flags, ctx);
            logger::info("Resizing descriptor buffer with new_size: ", data_size);
            std::memcpy(mapped, data, data_size);
        }
    }

    void DescriptorBuffer::deallocate(const types::detail::VulkanContext& ctx) {
        ctx.device_details.device.unmapMemory(memory);

        ctx.device_details.device.freeMemory(memory);
        ctx.device_details.device.destroyBuffer(buffer);
    }
} // namespace caelus::core::components::buffers