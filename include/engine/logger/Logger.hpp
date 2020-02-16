#ifndef CAELUS_LOGGER_HPP
#define CAELUS_LOGGER_HPP

#include "engine/core/Callbacks.hpp"
#include "fmt/format.h"

#include <string>

namespace caelus::engine::logger {
#if !defined(CAELUS_DISABLE_LOGGING)
    template <typename ...Args>
    void info(Args&& ...args) {
        std::cout << fmt::format(
            "[{}] [Logger] [Info]: ",
            core::callbacks::get_current_timestamp());

        (((std::cout << args), ...), std::cout << "\n");
    }

    template <typename ...Args>
    void warning(Args&& ...args) {
        std::cout << fmt::format(
            "[{}] [Logger] [Warning]: ",
            core::callbacks::get_current_timestamp());

        (((std::cout << args), ...), std::cout << "\n");
    }

    template <typename ...Args>
    void error(Args&& ...args) {
        std::cout << fmt::format(
            "[{}] [Logger] [Error]: ",
            core::callbacks::get_current_timestamp());

        (((std::cout << args), ...), std::cout << "\n");
    }
#else
    template <typename ...Args>
    void info([[maybe_unused]] Args&& ...args) {}

    template <typename ...Args>
    void warning([[maybe_unused]] Args&& ...args) {}

    template <typename ...Args>
    void error([[maybe_unused]] Args&& ...args) {}
#endif
} // namespace caelus::engine

#endif //CAELUS_LOGGER_HPP
