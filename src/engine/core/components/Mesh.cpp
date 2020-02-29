#include "engine/core/components/Mesh.hpp"

namespace caelus::core::components {
    void create_mesh(Mesh& mesh, types::detail::VulkanContext& ctx) {
        mesh.instance_buffer = buffers::make_generic_buffer(&ctx, vk::BufferUsageFlagBits::eStorageBuffer, mesh.instances);
        mesh.instance_descriptor = vulkan::create_descriptor_set(
            &ctx,
            mesh.pipeline.layout.set_layout,
            vk::DescriptorType::eStorageBuffer,
            vulkan::DescriptorBinding::Instance);

        vulkan::update_descriptors(mesh.instance_descriptor, buffers::get_buffer_info(mesh.instance_buffer));
    }
}