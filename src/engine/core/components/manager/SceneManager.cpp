#include "engine/core/components/managers/SceneManager.hpp"
#include "engine/core/components/buffers/VertexBuffer.hpp"
#include "engine/core/components/Scene.hpp"
#include "engine/core/vulkan/Pipeline.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::components::managers {
    const Scene& SceneManager::get_scene(const u64 id) const {
        return scenes.at(id);
    }

    void SceneManager::remove_scene(const u64 id) {
        scenes.erase(id);
    }

    Scene& SceneManager::operator [](const u64 id) {
        if (scenes.contains(id)) {
            logger::warning("Scene with id: ", id, " already exists, overwriting.");
        }

        return scenes[id];
    }
} // namespace caelus::core::components::managers