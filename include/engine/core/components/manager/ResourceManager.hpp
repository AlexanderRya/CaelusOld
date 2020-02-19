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
        static inline std::vector<components::Mesh> meshes{};
        static inline std::unordered_map<u32, vulkan::Pipeline> pipelines{};
    public:
        ResourceManager() = delete;

        static void add_mesh(const components::Mesh&);
        static void add_pipeline(const types::info::PipelineCreateInfo&);

        [[nodiscard]] static vulkan::Pipeline get_pipeline(const u32);
        [[nodiscard]] static const std::vector<components::Mesh>& get_meshes();
    };
} // namespace caelus::core::manager

#endif //CAELUS_RESOURCEMANAGER_HPP
