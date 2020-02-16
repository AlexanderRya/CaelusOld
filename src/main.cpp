#include "engine/core/Application.hpp"

int main() {
    caelus::engine::core::Application app{};
    app.init(1280, 720, "Engine");
    app.run();
    return 0;
}
