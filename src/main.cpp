#include "engine/core/Application.hpp"
#include <dlfcn.h>

int main() {
    caelus::core::Application app{};
    app.init();
    app.run();
    return 0;
}
