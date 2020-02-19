#include "engine/core/components/manager/ResourceManager.hpp"
#include "engine/core/info/PipelineCreateInfo.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::manager {
    void ResourceManager::add_mesh(const components::Mesh& mesh) {
        meshes.emplace_back(mesh);
    }

    void ResourceManager::add_pipeline(const types::info::PipelineCreateInfo& info) {
        if (pipelines.contains(info.id)) {
            logger::warning("Pipeline with id: ", info.id, " already exists, overwriting.");
        }

        pipelines[info.id].create(info);
    }

    vulkan::Pipeline ResourceManager::get_pipeline(const u32 id) const {
        return pipelines.at(id);
    }

    const std::vector<components::Mesh>& ResourceManager::get_meshes() const {
        return meshes;
    }
} // namespace caelus::core::manager