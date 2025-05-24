#include <ostream>
#include <type_traits>

namespace probare
{

    // Check if value is streamable
    template <typename T, typename = void>
    struct IsStreamable : std::false_type {};

    template <typename T>
    struct IsStreamable<T, std::void_t<decltype(std::declval<std::ostream &>()
                                                << std::declval<T>())>>
        : std::true_type {};
} // namespace probare
