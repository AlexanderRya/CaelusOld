#include "engine/core/components/Scene.hpp"
#include "engine/core/components/Mesh.hpp"
#include "vulkan/vulkan.hpp"
#include <cmath>

namespace caelus::core::components {
    void Scene::add_mesh(const Mesh& mesh) {
        meshes.emplace_back(mesh);
    }

    const Mesh& Scene::get_mesh(const usize idx) const {
        return meshes[idx];
    }

    const std::vector<Mesh>& Scene::get_meshes() const {
        return meshes;
    }
} // namespace caelus::core::components