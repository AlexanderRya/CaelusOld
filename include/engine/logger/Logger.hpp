#ifndef CAELUS_LOGGER_HPP
#define CAELUS_LOGGER_HPP

#include "engine/core/util/Util.hpp"

#include <iostream>
#include <string>

namespace caelus::logger {
#if !defined(CAELUS_DISABLE_LOGGING)
    template <typename ...Args>
    void info(Args&& ...args) {
        std::cout << core::util::format(
            "[{}] [Logger] [Info]: ",
            core::util::get_current_timestamp());

        (((std::cout << args), ...), std::cout << "\n");
    }

    template <typename ...Args>
    void warning(Args&& ...args) {
        std::cout << core::util::format(
            "[{}] [Logger] [Warning]: ",
            core::util::get_current_timestamp());

        (((std::cout << args), ...), std::cout << "\n");
    }

    template <typename ...Args>
    void error(Args&& ...args) {
        std::cout << core::util::format(
            "[{}] [Logger] [Error]: ",
            core::util::get_current_timestamp());

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
} // namespace caelus::logger

#endif //CAELUS_LOGGER_HPP
