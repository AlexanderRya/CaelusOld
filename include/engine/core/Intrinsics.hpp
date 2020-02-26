#ifndef CAELUS_INTRINSICS_HPP
#define CAELUS_INTRINSICS_HPP

namespace caelus::core::util {
#if _WIN32
    #define vulkan_module "vulkan-1.dll"
    void* load_module(LPCWSTR name) {
        return LoadLibraryW(name);
    }
    void (*load_symbol(HMODULE handle, LPCWSTR symbol))() {
        return reinterpret_cast<void(*)()>(GetProcAddressW(handle, symbol));
    }
    void close_module(HMODULE handle) {
        FreeLibrary(module)
    }
#elif __linux__
    #define vulkan_module "libvulkan.so"
    void* load_module(const char* name) {
        return dlopen(name, RTLD_LAZY | RTLD_LOCAL);
    }
    void (*load_symbol(void* handle, const char* symbol))() {
        return reinterpret_cast<void(*)()>(dlsym(handle, symbol));
    }
    void close_module(void* handle) {
        dlclose(handle);
    }
#endif
} // namespace caelus::core::util

#endif //CAELUS_INTRINSICS_HPP
