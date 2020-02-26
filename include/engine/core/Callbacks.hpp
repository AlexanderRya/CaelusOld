#ifndef CAELUS_CALLBACKS_HPP
#define CAELUS_CALLBACKS_HPP

#include <string>
#include <ctime>
#include <chrono>
#include <iostream>

#include "vulkan/vulkan.hpp"

namespace caelus::core::callbacks {
    [[maybe_unused]] VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void*);

    [[maybe_unused]] void glfw_error_callback(int code, const char* message);
} // namespace caelus::core::callbacks

#endif //CAELUS_CALLBACKS_HPP
