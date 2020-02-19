#ifndef CAELUS_DESCRIPTORSET_HPP
#define CAELUS_DESCRIPTORSET_HPP

#include "engine/core/info/DescriptorSetCreateInfo.hpp"
#include "engine/experimental/BufferContainer.hpp"
#include "engine/core/Constants.hpp"
#include "vulkan/vulkan.hpp"
#include <array>

namespace caelus::core::types::info {
    struct DescriptorSetCreateInfo;
} // namespace caelus::core::types::info

namespace caelus::core::vulkan {
    struct DescriptorSet {
        experimental::components::buffers::BufferType buffer;
        vk::DescriptorSet descriptor_set;

        void create(const types::info::DescriptorSetCreateInfo&);

        template <typename Ty>
        void write(const Ty&);
    };

    template <typename Ty>
    void DescriptorSet::write(const Ty& data) {
        switch (buffer.type) {
            case experimental::components::buffers::BufferType::ActiveType::UniformBuffer: {
                buffer.uniform_buffer.write(data);
            } break;
        }
    }

    std::array<DescriptorSet, constants::max_frames_in_flight> create_sets(const types::info::DescriptorSetCreateInfo&);
} // namespace caelus::core::vulkan

#endif //CAELUS_DESCRIPTORSET_HPP
