#include "engine/core/components/buffers/DescriptorBuffer.hpp"
#include "engine/core/components/manager/ResourceManager.hpp"
#include "engine/core/renderer/vulkan/DescriptorSet.hpp"
#include "engine/core/renderer/vulkan/Pipeline.hpp"
#include "engine/core/components/Mesh.hpp"
#include "engine/core/info/Info.hpp"
#include "engine/logger/Logger.hpp"

namespace caelus::core::manager {
    std::vector<components::Mesh> ResourceManager::meshes = {};
    std::unordered_map<u32, vulkan::Pipeline> ResourceManager::pipelines = {};

    std::unordered_map<u32, std::vector<components::buffers::DescriptorBuffer>> ResourceManager::descriptor_buffers = {};
    std::unordered_map<u32, std::vector<vulkan::DescriptorSet>> ResourceManager::descriptor_sets = {};

    void ResourceManager::add_mesh(const components::Mesh& mesh) {
        meshes.emplace_back(mesh);
    }

    void ResourceManager::add_pipeline(const types::info::PipelineCreateInfo& info) {
        if (pipelines.contains(info.id)) {
            logger::warning("Pipeline with id: ", info.id, " already exists, overwriting.");
        }

        pipelines[info.id].create(info);
    }

    void ResourceManager::add_descriptor_buffers(
        const u32 id,
        const usize size,
        const vk::BufferUsageFlags& flags,
        const types::detail::VulkanContext& ctx) {

        if (descriptor_buffers.contains(id)) {
            logger::warning("Descriptor buffer with id: ", id, " already exists, overwriting.");
        }

        descriptor_buffers[id].resize(constants::max_frames_in_flight);

        for (auto& buf : descriptor_buffers[id]) {
            buf.allocate(size, flags, ctx);
        }
    }

    void ResourceManager::add_descriptor_sets(const u32 id, const types::info::DescriptorSetInfo& info) {
        if (descriptor_sets.contains(id)) {
            logger::warning("Descriptor set with id: ", id, " already exists, overwriting.");
        }

        descriptor_sets[id] = vulkan::make_descriptor_sets(info);
    }

    vulkan::Pipeline ResourceManager::get_pipeline(const u32 id) {
        return pipelines.at(id);
    }

    const std::vector<components::Mesh>& ResourceManager::get_meshes() {
        return meshes;
    }

    std::vector<components::buffers::DescriptorBuffer>& ResourceManager::get_descriptor_buffers(const u32 id) {
        return descriptor_buffers.at(id);
    }

    std::vector<vulkan::DescriptorSet>& ResourceManager::get_descriptor_sets(const u32 id) {
        return descriptor_sets.at(id);
    }
} // namespace caelus::core::manager