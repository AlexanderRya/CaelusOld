cmake_minimum_required(VERSION 3.15)
project(Caelus)

find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw)
add_subdirectory(external/fmt)
add_subdirectory(external/glm)

include_directories(
        include
        external/glm
        external/fmt/include
        external/glfw/include
        ${Vulkan_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)

set(CAELUS_HEADERS
        include/engine/core/renderer/Renderer.hpp
        include/engine/core/Window.hpp
        include/engine/core/Types.hpp
        include/engine/core/vulkan/Device.hpp
        include/engine/core/Callbacks.hpp
        include/engine/core/details/Details.hpp
        include/engine/core/vulkan/Instance.hpp
        include/engine/core/vulkan/Swapchain.hpp
        include/engine/logger/Logger.hpp
        include/engine/core/Application.hpp
        include/engine/core/vulkan/CommandPool.hpp
        include/engine/core/vulkan/DescriptorPool.hpp
        include/engine/core/vulkan/RenderPass.hpp
        include/engine/core/Constants.hpp
        include/engine/core/vulkan/Framebuffer.hpp
        include/engine/core/vulkan/Pipeline.hpp
        include/engine/core/util/Util.hpp
        include/engine/core/components/Mesh.hpp
        include/engine/core/components/Texture.hpp
        include/engine/core/vulkan/CommandBuffer.hpp
        include/engine/core/vulkan/Fence.hpp
        include/engine/core/vulkan/Semaphore.hpp
        include/engine/core/components/buffers/VertexBuffer.hpp
        include/engine/core/components/Camera.hpp
        include/engine/core/components/managers/SceneManager.hpp
        include/engine/core/vulkan/VulkanContext.hpp
        include/engine/core/vulkan/PipelineLayout.hpp
        include/engine/core/components/Scene.hpp)

set(CAELUS_SOURCES
        src/main.cpp
        src/engine/core/Window.cpp
        src/engine/core/renderer/Renderer.cpp
        src/engine/core/vulkan/Device.cpp
        src/engine/core/vulkan/Instance.cpp
        src/engine/core/vulkan/Swapchain.cpp
        src/engine/core/Application.cpp
        src/engine/core/vulkan/CommandPool.cpp
        src/engine/core/vulkan/RenderPass.cpp
        src/engine/core/vulkan/Framebuffer.cpp
        src/engine/core/vulkan/DescriptorPool.cpp
        src/engine/core/vulkan/Pipeline.cpp
        src/engine/core/Callbacks.cpp
        src/engine/core/vulkan/CommandBuffer.cpp
        src/engine/core/vulkan/Fence.cpp
        src/engine/core/vulkan/Semaphore.cpp
        src/engine/core/components/buffers/VertexBuffer.cpp
        src/engine/core/util/Util.cpp
        src/engine/core/components/Camera.cpp
        src/engine/core/vulkan/VulkanContext.cpp
        src/engine/core/components/manager/SceneManager.cpp
        src/engine/core/components/Scene.cpp)

add_executable(Caelus
        ${CAELUS_HEADERS}
        ${CAELUS_SOURCES})

target_compile_options(Caelus PRIVATE -std=c++20 -O3 -Wall -Wextra -Werror -pedantic -std=libc++)

if (WIN32)
    target_compile_definitions(Caelus PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if (UNIX)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        target_compile_definitions(Caelus PRIVATE _GLIBCXX_DEBUG)
    endif()
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Caelus PRIVATE CAELUS_DEBUG)
else()
    target_compile_definitions(Caelus PRIVATE CAELUS_DISABLE_LOGGING)
endif()

if (UNIX)
    set_target_properties(
            glfw fmt
            PROPERTIES
            COMPILE_OPTIONS -Wno-everything)
endif()

if (WIN32)
    target_link_libraries(Caelus ${Vulkan_LIBRARIES} glfw fmt)
elseif (UNIX)
    target_link_libraries(Caelus ${Vulkan_LIBRARIES} glfw fmt pthread c++ c++abi)
endif()