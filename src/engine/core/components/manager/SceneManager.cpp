#include "engine/core/components/managers/SceneManager.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::components::managers {
    void SceneManager::add_scene(const u64 id, const Scene& info) {
        if (scenes.contains(id)) {
            logger::warning("Scene with id: ", id, " already exists, overwriting.");
        }

        scenes[id] = info;
    }

    void SceneManager::remove_scene(const u64 id) {
        scenes.erase(id);
    }

    Scene& SceneManager::operator [](const u64 id) {
        return scenes[id];
    }

    const Scene& SceneManager::operator [](const u64 id) const {
        return scenes.at(id);
    }
} // namespace caelus::core::components::managers