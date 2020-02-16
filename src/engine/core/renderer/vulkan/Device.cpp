#include "vulkan/vulkan.hpp"
#include "engine/logger/Logger.hpp"
#include "engine/core/renderer/Renderer.hpp"
#include "engine/core/details/VulkanData.hpp"
#include "engine/core/renderer/vulkan/Device.hpp"

namespace caelus::engine::core::vulkan {
    static inline vk::PhysicalDevice get_physical_device(const vk::Instance& instance) {
        auto physical_devices = instance.enumeratePhysicalDevices();

        for (const auto& device : physical_devices) {
            auto device_properties = device.getProperties();
            if (device_properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu ||
                device_properties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu) {
                engine::logger::info("Selected physical device: ", device_properties.deviceName);
                return device;
            }
        }

        throw std::runtime_error("No suitable physical device available");
    }

    static inline u32 get_queue_family(const vk::SurfaceKHR& surface, const vk::PhysicalDevice& physical_device) {
        auto queue_family_properties = physical_device.getQueueFamilyProperties();

        for (u32 i = 0; i < queue_family_properties.size(); ++i) {
            if (((queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics) &&
                physical_device.getSurfaceSupportKHR(i, surface)) {
                return i;
            }
        }

        throw std::runtime_error("Failed to find a queue family");
    }

    static inline vk::Device get_device(const u32 queue_family, const vk::PhysicalDevice& physical_device) {
        auto extensions = physical_device.enumerateDeviceExtensionProperties();
        constexpr const char* surface_ext[]{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        if (std::find_if(extensions.begin(), extensions.end(), [](const vk::ExtensionProperties& properties) {
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

            return physical_device.createDevice(device_create_info);
        } else {
            throw std::runtime_error("Selected physical device does not support a swapchain");
        }
    }

    static inline vk::Queue get_queue(const vk::Device& device, const u32 queue_family) {
        return device.getQueue(queue_family, 0);
    }

    types::detail::DeviceDetails get_device_details(const types::detail::VulkanData& data) {
        types::detail::DeviceDetails device_details{};

        device_details.physical_device = get_physical_device(data.instance);
        device_details.queue_family = get_queue_family(data.surface, device_details.physical_device);
        device_details.device = get_device(device_details.queue_family, device_details.physical_device);
        device_details.queue = get_queue(device_details.device, device_details.queue_family);

        return device_details;
    }
} // namespace caelus::engine::core::vulkan