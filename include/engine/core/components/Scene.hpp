#ifndef CAELUS_SCENE_HPP
#define CAELUS_SCENE_HPP

#include <vector>
#include "engine/core/Types.hpp"

namespace vk {
    class CommandBuffer;
} // namespace vk

namespace caelus::core::components {
    struct Mesh;

    class Scene {
        std::vector<Mesh> meshes;
    public:
        Scene() = default;

        void add_mesh(const Mesh&);
        const Mesh& get_mesh(const usize) const;
        const std::vector<Mesh>& get_meshes() const;
    };
} // namespace caelus::core::components

#endif //CAELUS_SCENE_HPP
