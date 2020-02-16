#ifndef CAELUS_CALLBACKS_HPP
#define CAELUS_CALLBACKS_HPP

#include <string>
#include <ctime>
#include <chrono>
#include <iostream>

#include "vulkan/vulkan.hpp"
#include "fmt/format.h"

namespace caelus::engine::core::callbacks {
    static std::string get_current_timestamp() {
        namespace ch = std::chrono;

        auto time = ch::duration_cast<ch::seconds>(ch::system_clock::now().time_since_epoch()).count();

        std::string buf(128, '\0');
        buf.resize(std::strftime(buf.data(), buf.size(), "%Y-%m-%d %X", std::localtime(&time)));

        return buf;
    }

    static std::string get_message_type(const VkDebugUtilsMessageTypeFlagsEXT& type) {
        switch (type) {
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: {
                return "General";
            }

            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: {
                return "Validation";
            }

            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: {
                return "Performance";
            }

            default: {
                return "Unknown";
            }
        }
    }

    static std::string get_message_severity(const VkDebugUtilsMessageSeverityFlagBitsEXT& type) {
        switch (type) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
                return "Verbose";
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
                return "Info";
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
                return "Warning";
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
                return "Error";
            }

            default: {
                return "Unknown";
            }
        }
    }

    [[maybe_unused]] static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void*) {

        std::cout << fmt::format(
            "[{}] [Vulkan] [{}/{}]: {}\n",
            get_current_timestamp(),
            get_message_severity(message_severity),
            get_message_type(message_type),
            callback_data->pMessage);

        if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            throw std::runtime_error("Validation layer error");
        }

        return 0;
    }

    [[maybe_unused]] static void glfw_error_callback(int code, const char* message) {
        std::cout << fmt::format(
            "[{}] [GLFW3] [Error: {}]: {}\n",
            get_current_timestamp(),
            code,
            message);
    }
} // namespace caelus::engine::core::callbacks

#endif //CAELUS_CALLBACKS_HPP
