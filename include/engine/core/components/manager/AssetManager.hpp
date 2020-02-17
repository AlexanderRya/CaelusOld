#ifndef CAELUS_ASSETMANAGER_HPP
#define CAELUS_ASSETMANAGER_HPP

#include "engine/core/renderer/vulkan/Pipeline.hpp"
#include "engine/core/components/Mesh.hpp"

#include <vector>
#include <unordered_map>

namespace caelus::core::types::info {
    struct PipelineCreateInfo;
} // caelus::core::types::info

namespace caelus::core::manager {
    class AssetManager {
        std::vector<components::Mesh> meshes;
        std::unordered_map<u32, vulkan::Pipeline> pipelines;
    public:
        AssetManager() = default;

        void add_mesh(const components::Mesh&);
        void add_pipeline(const types::info::PipelineCreateInfo&);

        [[nodiscard]] vulkan::Pipeline get_pipeline(const u32) const;
        [[nodiscard]] const std::vector<components::Mesh>& get_meshes() const;
    };
} // namespace caelus::core::managers

#endif //CAELUS_ASSETMANAGER_HPP
