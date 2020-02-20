#ifndef CAELUS_DETAILS_HPP
#define CAELUS_DETAILS_HPP

#include "engine/core/Types.hpp"
#include "vulkan/vulkan.hpp"

#include <vector>
#include <unordered_map>

namespace caelus::core::types::detail {
    struct DeviceDetails {
        u32 queue_family{};
        vk::Queue queue{};

        vk::PhysicalDevice physical_device{};
        vk::Device device{};
    };

    struct SwapchainDetails {
        vk::SwapchainKHR swapchain{};
        u32 image_count{};

        vk::Extent2D extent{};
        vk::SurfaceFormatKHR format{};
        vk::PresentModeKHR present_mode{};
        vk::SurfaceTransformFlagBitsKHR surface_transform{};

        std::vector<vk::Image> images{};
        std::vector<vk::ImageView> image_views{};
    };

    struct PipelineLayout {
        vk::PipelineLayout pipeline_layout{};
        vk::DescriptorSetLayout set_layout{};
    };

    struct DescriptorSetDetails {
        u32 buffer_idx;
        u32 buffer_id;
        u32 binding;

        vk::DescriptorSet descriptor_set;
        vk::DescriptorType type;
    };
} // namespace caelus::core::types::detail

#endif //CAELUS_DETAILS_HPP
