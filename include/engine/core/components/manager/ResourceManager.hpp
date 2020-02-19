#ifndef CAELUS_RESOURCEMANAGER_HPP
#define CAELUS_RESOURCEMANAGER_HPP

#include "engine/core/renderer/vulkan/Pipeline.hpp"
#include "engine/core/components/Mesh.hpp"

#include <vector>
#include <unordered_map>

namespace caelus::core::types::info {
    struct PipelineCreateInfo;
} // caelus::core::types::info

namespace caelus::core::manager {
    class ResourceManager {
        std::vector<components::Mesh> meshes;
        std::unordered_map<u32, vulkan::Pipeline> pipelines;
    public:
        ResourceManager() = default;

        void add_mesh(const components::Mesh&);
        void add_pipeline(const types::info::PipelineCreateInfo&);

        [[nodiscard]] vulkan::Pipeline get_pipeline(const u32) const;
        [[nodiscard]] const std::vector<components::Mesh>& get_meshes() const;
    };
} // namespace caelus::core::manager

#endif //CAELUS_RESOURCEMANAGER_HPP
