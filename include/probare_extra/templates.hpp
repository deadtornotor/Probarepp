#include <ostream>
#include <type_traits>

namespace probare
{

    // Check if value is streamable
    template <typename T, typename = void>
    struct is_streamable : std::false_type {};

    template <typename T>
    struct is_streamable<T, std::void_t<decltype(std::declval<std::ostream &>()
                                                 << std::declval<T>())>>
        : std::true_type {};
} // namespace probare
