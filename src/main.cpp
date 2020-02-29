#include "engine/core/Application.hpp"

int main() {
    caelus::core::Application app{};
    app.init();
    app.run();
    return 0;
}
