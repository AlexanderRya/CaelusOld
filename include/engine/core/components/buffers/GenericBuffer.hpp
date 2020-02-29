#ifndef CAELUS_GENERICBUFFER_HPP
#define CAELUS_GENERICBUFFER_HPP

#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/util/Util.hpp"
#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"
#include <vector>
#include <engine/core/Constants.hpp>

namespace caelus::core::components::buffers {
    template <typename Ty>
    class SingleGenericBuffer {
        vk::Buffer buffer{};
        vk::DeviceMemory memory{};
        vk::BufferUsageFlags flags{};

        void* mapped{};

        usize current_size{};
        usize capacity{};

        const types::detail::VulkanContext* ctx{};
    public:
        SingleGenericBuffer() = default;

        void init(const types::detail::VulkanContext*, const vk::BufferUsageFlags&);
        void allocate(const usize);
        void write(const std::vector<Ty>&);
        void write(const Ty&);
        void deallocate();

        [[nodiscard]] usize size() const;

        [[nodiscard]] vk::DescriptorBufferInfo get_info() const;
        [[nodiscard]] vk::Buffer handle() const;
    };

    template <typename Ty>
    void SingleGenericBuffer<Ty>::init(const types::detail::VulkanContext* context, const vk::BufferUsageFlags& usage_flags) {
        ctx = context;
        flags = usage_flags;

        allocate(1);
    }

    template <typename Ty>
    void SingleGenericBuffer<Ty>::allocate(const usize alloc_size) {
        buffer = util::make_buffer(alloc_size * sizeof(Ty), flags, *ctx);
        memory = util::allocate_memory(buffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, *ctx);
        mapped = ctx->device_details.device.mapMemory(memory, 0, sizeof(Ty) * alloc_size, {}, ctx->dispatcher);
        capacity = current_size = alloc_size;
    }

    template <typename Ty>
    void SingleGenericBuffer<Ty>::write(const std::vector<Ty>& data) {
        if (data.empty()) {
            throw std::runtime_error("Can't write an empty buffer");
        }

        if (data.size() > capacity) {
            deallocate();
            allocate(data.capacity());
        }

        std::memcpy(mapped, data.data(), sizeof(Ty) * data.size());
        current_size = data.size();
        capacity = data.capacity();
    }

    template <typename Ty>
    void SingleGenericBuffer<Ty>::write(const Ty& data) {
        std::memcpy(mapped, &data, sizeof(Ty));
        current_size = capacity = 1;
    }

    template <typename Ty>
    void SingleGenericBuffer<Ty>::deallocate() {
        ctx->device_details.device.unmapMemory(memory, ctx->dispatcher);

        ctx->device_details.device.freeMemory(memory, nullptr, ctx->dispatcher);
        ctx->device_details.device.destroyBuffer(buffer, nullptr, ctx->dispatcher);
    }

    template <typename Ty>
    usize SingleGenericBuffer<Ty>::size() const {
        return current_size;
    }

    template <typename Ty>
    vk::DescriptorBufferInfo SingleGenericBuffer<Ty>::get_info() const {
        vk::DescriptorBufferInfo buffer_info{}; {
            buffer_info.buffer = buffer;
            buffer_info.offset = 0;
            buffer_info.range = current_size * sizeof(Ty);
        }

        return buffer_info;
    }

    template <typename Ty>
    vk::Buffer SingleGenericBuffer<Ty>::handle() const {
        return buffer;
    }

    template <typename Ty>
    using GenericBuffer = std::vector<SingleGenericBuffer<Ty>>;

    template <typename Ty>
    [[nodiscard]] GenericBuffer<Ty> make_generic_buffer(
        const types::detail::VulkanContext* context,
        const vk::BufferUsageFlags& usage_flags,
        const std::vector<Ty>& data) {

        GenericBuffer<Ty> buffer(constants::frames_in_flight);

        for (auto& each : buffer) {
            each.init(context, usage_flags);
            each.write(data);
        }

        return buffer;
    }

    template <typename Ty>
    [[nodiscard]] GenericBuffer<Ty> make_generic_buffer(
        const types::detail::VulkanContext* context,
        const vk::BufferUsageFlags& usage_flags,
        const Ty& data) {

        GenericBuffer<Ty> buffer(constants::frames_in_flight);

        for (auto& each : buffer) {
            each.init(context, usage_flags);
            each.write(data);
        }

        return buffer;
    }

    template <typename Ty>
    [[nodiscard]] std::vector<vk::DescriptorBufferInfo> get_buffer_info(const GenericBuffer<Ty>& buffer) {
        std::vector<vk::DescriptorBufferInfo> infos{};
        infos.reserve(buffer.size());

        for (const auto& each : buffer) {
            infos.emplace_back(each.get_info());
        }

        return infos;
    }
} // namespace caelus::core::components::buffers

#endif //CAELUS_GENERICBUFFER_HPP
