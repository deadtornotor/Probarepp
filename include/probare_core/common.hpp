#pragma once

#include <mutex>
#include <ostream>
#include <sstream>
#include <type_traits>

#include "data.hpp"

namespace probare
{

    // Check if value is streamable
    template <typename T, typename = void>
    struct IsStreamable : std::false_type {};

    template <typename T>
    struct IsStreamable<T, std::void_t<decltype(std::declval<std::ostream &>()
                                                << std::declval<T>())>>
        : std::true_type {};

    std::mutex &message_mutex();

    void send_message(const std::string &message_);
    void send_message(const std::ostream &oss_);

    void send_error(const std::string &message_);
    void send_error(const std::ostream &oss_);
} // namespace probare

#ifndef PROBARE_MESSAGE

#define PROBARE_MESSAGE(message)                                               \
    {                                                                          \
        ::probare::send_message(message);                                      \
    }

#endif

#ifndef PROBARE_ERROR

#define PROBARE_ERROR(message)                                                 \
    {                                                                          \
        ::probare::send_error(message);                                        \
    }

#endif

#ifndef _PROBARE_HELPERS
#define _PROBARE_HELPERS

#define _PROBARE_SCOPE(scope_func)                                             \
    do {                                                                       \
        scope_func                                                             \
    } while (0);

#define _PROBARE_FAILED                                                        \
    {                                                                          \
        ::probare::stats().add_failed();                                       \
    }

#define _PROBARE_PASSED                                                        \
    {                                                                          \
        ::probare::stats().add_passed();                                       \
    }

#endif
