#ifndef CAELUS_BUFFERCONTAINER_HPP
#define CAELUS_BUFFERCONTAINER_HPP

#include "engine/core/components/buffers/UniformBuffer.hpp"
#include "engine/core/Types.hpp"
#include <utility>
#include <vector>

namespace caelus::experimental::components::buffers {
    struct BufferType {
        union {
            core::components::buffers::UniformBuffer uniform_buffer{};
        };

        enum class ActiveType {
            UniformBuffer
        } type{};
    };
} // namespace caelus::experimental::components::buffers

namespace caelus::experimental::components::containers {
    class BufferContainer {
        std::vector<buffers::BufferType> buffers;
    public:
        BufferContainer() = default;

        template <typename ...Args>
        decltype(auto) emplace_back(Args&& ...args) {
            return buffers.emplace_back(std::forward<Args>(args)...);
        }

        decltype(auto) operator [](const usize idx) {
            return buffers[idx];
        }

        decltype(auto) operator [](const usize idx) const {
            return buffers[idx];
        }
    };
} // namespace caelus::experimental::components::containers

#endif //CAELUS_BUFFERCONTAINER_HPP
