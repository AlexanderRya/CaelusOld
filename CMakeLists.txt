cmake_minimum_required(VERSION 3.15)
project(Caelus)

find_package(Vulkan REQUIRED)

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/fmt)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)

set(CAELUS_HEADERS
        include)

set(CAELUS_SOURCES
        src/main.cpp)

add_executable(Caelus
        ${CAELUS_HEADERS}
        ${CAELUS_SOURCES})

target_compile_options(Caelus PRIVATE -std=c++2a -O3 -Wall -Wextra -Werror -pedantic)

if (WIN32)
    target_compile_definitions(VkMinecraftPP PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if (UNIX)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        target_compile_definitions(VkMinecraftPP PRIVATE _GLIBCXX_DEBUG)
    endif()
endif()

#target_compile_definitions(VkMinecraftPP PRIVATE GLM_FORCE_LEFT_HANDED)

if (UNIX)
    set_target_properties(
            glfw fmt
            PROPERTIES
            COMPILE_OPTIONS -Wno-everything)
endif()

if (WIN32)
    target_link_libraries (VkMinecraftPP ${Vulkan_LIBRARIES} glfw fmt)
elseif (UNIX)
    target_link_libraries (VkMinecraftPP ${Vulkan_LIBRARIES} glfw fmt pthread)
endif()