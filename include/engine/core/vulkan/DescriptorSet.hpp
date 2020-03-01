#ifndef CAELUS_DESCRIPTORSET_HPP
#define CAELUS_DESCRIPTORSET_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"
#include "VulkanContext.hpp"

namespace caelus::core::vulkan {
    enum class DescriptorBinding : u32 {
        Camera = 0,
        Instance = 1
    };

    class SingleDescriptorSet {
        vk::DescriptorSet descriptor{};
        vk::DescriptorSetLayout layout{};

        const types::detail::VulkanContext* ctx{};
    public:

        SingleDescriptorSet() = default;

        void create(const types::detail::VulkanContext*, const vk::DescriptorSetLayout&);

        void update(const vk::DescriptorBufferInfo&, const vk::DescriptorType, const DescriptorBinding&);

        [[nodiscard]] vk::DescriptorSet handle() const;
    };

    using DescriptorSet = std::vector<SingleDescriptorSet>;

    [[nodiscard]] DescriptorSet create_descriptor_set(const types::detail::VulkanContext*, const vk::DescriptorSetLayout&);
    void update_descriptors(DescriptorSet&, const std::vector<vk::DescriptorBufferInfo>&, const vk::DescriptorType, const DescriptorBinding&);
} // namespace caelus::core::vulkan

#endif //CAELUS_DESCRIPTORSET_HPP
