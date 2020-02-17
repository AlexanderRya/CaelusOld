#include "engine/core/components/manager/AssetManager.hpp"
#include "engine/core/info/PipelineCreateInfo.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::manager {
    void AssetManager::add_mesh(const components::Mesh& mesh) {
        meshes.emplace_back(mesh);
    }

    void AssetManager::add_pipeline(const types::info::PipelineCreateInfo& info) {
        if (pipelines.contains(info.id)) {
            logger::warning("Pipeline with id: ", info.id, " already exists.");
        }

        pipelines[info.id].create(info);
    }

    vulkan::Pipeline AssetManager::get_pipeline(const u32 id) const {
        return pipelines.at(id);
    }

    const std::vector<components::Mesh>& AssetManager::get_meshes() const {
        return meshes;
    }
} // namespace caelus::core::managers