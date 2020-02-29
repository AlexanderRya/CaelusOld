#include "engine/core/vulkan/VulkanContext.hpp"
#include "engine/core/vulkan/Swapchain.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/Window.hpp"
#include "vulkan/vulkan.hpp"

#include <iostream>

namespace caelus::core::vulkan {
    static inline u32 get_image_count(const vk::SurfaceCapabilitiesKHR& capabilities) {
        auto count = capabilities.minImageCount + 1;

        if (capabilities.maxImageCount > 0 && count > capabilities.maxImageCount) {
            count = capabilities.maxImageCount;
        }

        caelus::logger::info("Swapchain details: image count: ", count);

        return count;
    }

    static inline vk::Extent2D get_extent(const Window& window, const vk::SurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != UINT32_MAX) {

            return capabilities.currentExtent;
        } else {
            vk::Extent2D actual_extent{ window.width, window.height };

            actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actual_extent;
        }
    }

    static inline vk::SurfaceFormatKHR get_format(const types::detail::VulkanContext& ctx) {
        auto surface_formats = ctx.device_details.physical_device.getSurfaceFormatsKHR(ctx.surface, {}, ctx.dispatcher);

        vk::SurfaceFormatKHR format = surface_formats[0];

        for (const auto& each : surface_formats) {
            if (each.format == vk::Format::eB8G8R8A8Srgb &&
                each.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                caelus::logger::info("Swapchain details: format: vk::Format::", vk::to_string(each.format));
                caelus::logger::info("Swapchain details: color space: vk::ColorSpaceKHR::", vk::to_string(each.colorSpace));
                return each;
            }
        }

        caelus::logger::info("Swapchain details: non-preferred format: vk::Format::", vk::to_string(format.format));
        caelus::logger::info("Swapchain details: non-preferred color space: vk::ColorSpaceKHR::", vk::to_string(format.colorSpace));

        return format;
    }

    static inline vk::PresentModeKHR get_present_mode(const types::detail::VulkanContext& ctx) {
        for (const auto& mode : ctx.device_details.physical_device.getSurfacePresentModesKHR(ctx.surface, {}, ctx.dispatcher)) {
            if (mode == vk::PresentModeKHR::eImmediate) {
                caelus::logger::info("Swapchain details: present mode: vk::PresentModeKHR::", vk::to_string(mode));
                return mode;
            }
        }

        caelus::logger::warning("Swapchain details: non-preferreds present mode: vk::PresentModeKHR::", vk::to_string(vk::PresentModeKHR::eFifo));

        return vk::PresentModeKHR::eFifo;
    }

    static inline void get_swapchain(const types::detail::VulkanContext& ctx, types::detail::SwapchainDetails& details) {
        vk::SwapchainCreateInfoKHR swapchain_create_info{}; {
            swapchain_create_info.surface = ctx.surface;
            swapchain_create_info.minImageCount = details.image_count;
            swapchain_create_info.imageFormat = details.format.format;
            swapchain_create_info.imageColorSpace = details.format.colorSpace;
            swapchain_create_info.imageExtent = details.extent;
            swapchain_create_info.preTransform = details.surface_transform;
            swapchain_create_info.imageArrayLayers = 1;
            swapchain_create_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
            swapchain_create_info.imageSharingMode = vk::SharingMode::eExclusive;
            swapchain_create_info.queueFamilyIndexCount = 1;
            swapchain_create_info.pQueueFamilyIndices = &ctx.device_details.queue_family;
            swapchain_create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
            swapchain_create_info.presentMode = details.present_mode;
            swapchain_create_info.clipped = true;
            swapchain_create_info.oldSwapchain = nullptr;
        }

        details.swapchain = ctx.device_details.device.createSwapchainKHR(swapchain_create_info, nullptr, ctx.dispatcher);

        caelus::logger::info("Swapchain successfully created");
    }

    static inline void create_images(const types::detail::VulkanContext& ctx, types::detail::SwapchainDetails& details) {
        details.images = ctx.device_details.device.getSwapchainImagesKHR(details.swapchain, ctx.dispatcher);

        vk::ImageViewCreateInfo image_view_create_info{}; {
            image_view_create_info.format = details.format.format;
            image_view_create_info.components.r = vk::ComponentSwizzle::eIdentity;
            image_view_create_info.components.g = vk::ComponentSwizzle::eIdentity;
            image_view_create_info.components.b = vk::ComponentSwizzle::eIdentity;
            image_view_create_info.components.a = vk::ComponentSwizzle::eIdentity;
            image_view_create_info.viewType = vk::ImageViewType::e2D;
            image_view_create_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            image_view_create_info.subresourceRange.baseMipLevel = 0;
            image_view_create_info.subresourceRange.levelCount = 1;
            image_view_create_info.subresourceRange.baseArrayLayer = 0;
            image_view_create_info.subresourceRange.layerCount = 1;
        }

        details.image_views.reserve(details.image_count);

        for (const auto& image : details.images) {
            image_view_create_info.image = image;

            details.image_views.emplace_back(
                ctx.device_details.device.createImageView(image_view_create_info, nullptr, ctx.dispatcher));
        }

        caelus::logger::info("Swapchain images successfully created");
    }

    types::detail::SwapchainDetails get_swapchain_details(const Window& window, const types::detail::VulkanContext& ctx) {
        auto capabilities = ctx.device_details.physical_device.getSurfaceCapabilitiesKHR(ctx.surface, ctx.dispatcher);

        types::detail::SwapchainDetails details{};

        details.image_count = get_image_count(capabilities);
        details.extent = get_extent(window, capabilities);
        details.format = get_format(ctx);
        details.present_mode = get_present_mode(ctx);

        details.surface_transform = capabilities.currentTransform;

        get_swapchain(ctx, details);
        create_images(ctx, details);

        return details;
    }
} // namespace caelus::core::vulkan