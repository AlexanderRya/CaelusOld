#include "vulkan/vulkan.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/vulkan/Device.hpp"
#include "engine/core/vulkan/VulkanContext.hpp"

namespace caelus::core::vulkan {
    static inline vk::PhysicalDevice get_physical_device(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dispatcher) {
        auto physical_devices = instance.enumeratePhysicalDevices({}, dispatcher);

        for (const auto& device : physical_devices) {
            auto device_properties = device.getProperties(dispatcher);
            if (device_properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu ||
                device_properties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu) {
                caelus::logger::info("Selected physical device: ", device_properties.deviceName);
                return device;
            }
        }

        throw std::runtime_error("No suitable physical device available");
    }

    static inline u32 get_queue_family(const vk::SurfaceKHR& surface, const vk::PhysicalDevice& physical_device, const vk::DispatchLoaderDynamic& dispatcher) {
        auto queue_family_properties = physical_device.getQueueFamilyProperties({}, dispatcher);

        for (u32 i = 0; i < queue_family_properties.size(); ++i) {
            if (((queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics) &&
                physical_device.getSurfaceSupportKHR(i, surface, dispatcher)) {
                return i;
            }
        }

        throw std::runtime_error("Failed to find a queue family");
    }

    static inline vk::Device get_device(const u32 queue_family, const vk::PhysicalDevice& physical_device, const vk::DispatchLoaderDynamic& dispatcher) {
        auto extensions = physical_device.enumerateDeviceExtensionProperties(nullptr, {}, dispatcher);
        constexpr const char* surface_ext[]{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        if (std::find_if(extensions.begin(), extensions.end(), [](const vk::ExtensionProperties& properties){
            return std::strcmp(properties.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0;
        }) != extensions.end()) {
            float priorities[]{ 1.0f };

            vk::DeviceQueueCreateInfo queue_create_info{}; {
                queue_create_info.queueCount = 1;
                queue_create_info.queueFamilyIndex = queue_family;
                queue_create_info.pQueuePriorities = priorities;
            }

            vk::DeviceCreateInfo device_create_info{}; {
                device_create_info.ppEnabledExtensionNames = surface_ext;
                device_create_info.enabledExtensionCount = 1;
                device_create_info.pQueueCreateInfos = &queue_create_info;
                device_create_info.queueCreateInfoCount = 1;
            }

            return physical_device.createDevice(device_create_info, nullptr, dispatcher);
        } else {
            throw std::runtime_error("Selected physical device does not support a swapchain");
        }
    }

    static inline vk::Queue get_queue(const vk::Device& device, const u32 queue_family, const vk::DispatchLoaderDynamic& dispatcher) {
        return device.getQueue(queue_family, 0, dispatcher);
    }

    types::detail::DeviceDetails get_device_details(const types::detail::VulkanContext& ctx) {
        types::detail::DeviceDetails device_details{};

        device_details.physical_device = get_physical_device(ctx.instance, ctx.dispatcher);
        device_details.queue_family = get_queue_family(ctx.surface, device_details.physical_device, ctx.dispatcher);
        device_details.device = get_device(device_details.queue_family, device_details.physical_device, ctx.dispatcher);
        device_details.queue = get_queue(device_details.device, device_details.queue_family, ctx.dispatcher);

        return device_details;
    }

    u32 find_memory_type(const types::detail::VulkanContext& ctx, const u32 mask, const vk::MemoryPropertyFlags& flags) {
        const vk::PhysicalDeviceMemoryProperties memory_properties = ctx.device_details.physical_device.getMemoryProperties(ctx.dispatcher);

        for (u32 i = 0; i < memory_properties.memoryTypeCount; ++i) {
            if ((mask & (1u << i)) &&
                (memory_properties.memoryTypes[i].propertyFlags & flags) == flags) {
                return i;
            }
        }

        throw std::runtime_error("Failed to find memory type");
    }

} // namespace caelus::core::vulkan