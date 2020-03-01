#ifndef CAELUS_UTIL_HPP
#define CAELUS_UTIL_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"
#include <sstream>

namespace caelus::core::types::detail {
    struct VulkanContext;
} // namespace caelus::core::types::detail

namespace caelus::core::components::buffers {
    struct VertexBuffer;
} // namespace caelus::core::components::buffers

namespace caelus::core::util {
    [[nodiscard]] std::string get_current_timestamp();
    [[nodiscard]] vk::Buffer make_buffer(const usize, const vk::BufferUsageFlags&, const types::detail::VulkanContext&);
    [[nodiscard]] vk::DeviceMemory allocate_memory(const vk::Buffer&, const vk::MemoryPropertyFlags&, const types::detail::VulkanContext&);
    void copy_buffer(const vk::Buffer&, const vk::Buffer&, const usize, const types::detail::VulkanContext&);
    [[nodiscard]] std::vector<types::Vertex> generate_triangle_geometry();
    [[nodiscard]] std::vector<types::Vertex> generate_quad_geometry();

    template <typename ...Args>
    [[nodiscard]] std::string format(const std::string& str, Args&& ...args) {
        /* Check if argument count and format specifiers match */ {
            u64 start = 0;
            u64 count = 0;

            while ((start = str.find("{}", start)) != std::string::npos) {
                start += 2;
                ++count;
            }

            if (count != sizeof...(args)) {
                throw std::runtime_error("Format specifiers and argument count mismatch");
            }
        }

        auto internal_fmt = [](const std::string& istr, std::stringstream& oss, size_t & start_idx, const auto& var) {
            size_t index = istr.find("{}", start_idx);

            if (index == std::string::npos) {
                return;
            }

            oss << istr.substr(start_idx, index - start_idx) << var;
            start_idx = index + 2;
        };

        std::stringstream oss{};
        size_t start_idx = 0;
        (internal_fmt(str, oss, start_idx, std::forward<Args>(args)), ...);

        return oss << str.substr(start_idx, str.length()), oss.str();
    }
} // namespace caelus::core::util

#endif //CAELUS_UTIL_HPP
