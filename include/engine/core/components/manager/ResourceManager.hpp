#ifndef CAELUS_RESOURCEMANAGER_HPP
#define CAELUS_RESOURCEMANAGER_HPP

#include "engine/core/Constants.hpp"
#include "engine/core/Types.hpp"

#include <vector>
#include <unordered_map>

namespace caelus::core {
    namespace types::info {
        struct PipelineCreateInfo;
        struct DescriptorSetInfo;
    } // namespace caelus::core::types::info

    namespace types::detail {
        struct VulkanContext;
    } // namespace caelus::core::types::info

    namespace vulkan {
        struct Pipeline;
        struct DescriptorSet;
    } // namespace caelus::core::vulkan

    namespace components {
        struct Mesh;
        namespace buffers {
            struct DescriptorBuffer;
        } // namespace caelus::core::components::buffers
    } // namespace caelus::core::components
} // namespace caelus::core

namespace caelus::core::manager {
    class ResourceManager {
        static std::vector<components::Mesh> meshes;
        static std::unordered_map<u32, vulkan::Pipeline> pipelines;

        static std::unordered_map<u32, std::vector<components::buffers::DescriptorBuffer>> descriptor_buffers;
        static std::unordered_map<u32, std::vector<vulkan::DescriptorSet>> descriptor_sets;
    public:
        ResourceManager() = delete;

        static void add_mesh(const components::Mesh&);
        static void add_pipeline(const types::info::PipelineCreateInfo&);

        static void add_descriptor_buffers(const u32, const usize, const vk::BufferUsageFlags&, const types::detail::VulkanContext&);
        static void add_descriptor_sets(const u32, const types::info::DescriptorSetInfo&);

        [[nodiscard]] static vulkan::Pipeline get_pipeline(const u32);
        [[nodiscard]] static const std::vector<components::Mesh>& get_meshes();

        [[nodiscard]] static std::vector<components::buffers::DescriptorBuffer>& get_descriptor_buffers(const u32);
        [[nodiscard]] static std::vector<vulkan::DescriptorSet>& get_descriptor_sets(const u32);
    };
} // namespace caelus::core::manager

#endif //CAELUS_RESOURCEMANAGER_HPP
