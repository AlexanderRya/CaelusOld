#include "engine/core/renderer/vulkan/Instance.hpp"
#include "engine/core/details/VulkanData.hpp"
#include "engine/core/Callbacks.hpp"
#include "GLFW/glfw3.h"

namespace caelus::engine::core::vulkan {
    static inline std::vector<const char*> get_required_extensions() {
        u32 count = 0;

        auto required_extensions = glfwGetRequiredInstanceExtensions(&count);
        auto extensions = vk::enumerateInstanceExtensionProperties();

        std::vector<const char*> enabled_extensions;
        enabled_extensions.reserve(count + 1);

        for (u32 i = 0; i < count; ++i) {
            for (const auto& extension : extensions) {
                if (std::strcmp(extension.extensionName, required_extensions[i]) == 0) {
                    enabled_extensions.emplace_back(required_extensions[i]);
                    break;
                }
            }
        }

        if (enabled_extensions.size() != count) {
            throw std::runtime_error("Required extension not supported.");
        }
#ifdef CAELUS_DEBUG
        enabled_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
        return enabled_extensions;
    }

    vk::Instance get_instance() {
        vk::ApplicationInfo application_info{}; {
            application_info.apiVersion = VK_API_VERSION_1_2;
            application_info.applicationVersion = VK_API_VERSION_1_2;
            application_info.engineVersion = VK_API_VERSION_1_2;

            application_info.pEngineName = "Caelus Engine";
            application_info.pApplicationName = "";
        }

        auto enabled_exts = get_required_extensions();

        [[maybe_unused]] const char* validation_layer = "VK_LAYER_KHRONOS_validation";

        vk::InstanceCreateInfo instance_create_info{}; {
            instance_create_info.pApplicationInfo = &application_info;

            instance_create_info.ppEnabledExtensionNames = enabled_exts.data();
            instance_create_info.enabledExtensionCount = enabled_exts.size();
#ifdef CAELUS_DEBUG
            instance_create_info.ppEnabledLayerNames = &validation_layer;
            instance_create_info.enabledLayerCount = 1;
#else
            instance_create_info.ppEnabledLayerNames = nullptr;
            instance_create_info.enabledLayerCount = 0;
#endif
        }

        return vk::createInstance(instance_create_info);
    }

    vk::DebugUtilsMessengerEXT install_validation_layers(const types::detail::VulkanData& data) {
        vk::DebugUtilsMessengerCreateInfoEXT create_info{}; {
            create_info.messageSeverity =
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
            create_info.messageType =
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral    |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
            create_info.pfnUserCallback = callbacks::vulkan_debug_callback;
        }

        vk::DispatchLoaderDynamic dispatcher{ data.instance, vkGetInstanceProcAddr };

        return data.instance.createDebugUtilsMessengerEXT(create_info, nullptr, dispatcher);
    }
} // namespace caelus::engine::core::vulkan