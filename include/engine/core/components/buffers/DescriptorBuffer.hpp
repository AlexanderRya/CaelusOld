#ifndef CAELUS_DESCRIPTORBUFFER_HPP
#define CAELUS_DESCRIPTORBUFFER_HPP

#include "vulkan/vulkan.hpp"

namespace caelus::core::components::buffers {
    class DescriptorBuffer {
        std::vector<vk::Buffer> buffer;
        std::vector<vk::DeviceMemory> memory;
    public:
    };
} // namespace caelus::core::components::buffers

#endif //CAELUS_DESCRIPTORBUFFER_HPP
