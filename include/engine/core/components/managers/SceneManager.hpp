#ifndef CAELUS_SCENEMANAGER_HPP
#define CAELUS_SCENEMANAGER_HPP

#include "engine/core/components/Scene.hpp"
#include "engine/core/components/Mesh.hpp"
#include "engine/core/Types.hpp"
#include <unordered_map>

namespace caelus::core::components::managers {
    class SceneManager {
        std::unordered_map<u64, Scene> scenes;
    public:
        SceneManager() = default;

        const Scene& get_scene(const u64) const;
        void remove_scene(const u64);

        Scene& operator [](const u64);
    };
} // namespace caelus::core::components::managers

#endif //CAELUS_SCENEMANAGER_HPP
