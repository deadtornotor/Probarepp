#include "probare_core/common.hpp"

#include <iostream>
#include <sstream>

namespace probare
{
    std::mutex &message_mutex()
    {
        static std::mutex mutex;
        return mutex;
    }

    void send_message(const std::string &message_)
    {
        std::lock_guard<std::mutex> lock(message_mutex());
        std::cout << message_ << std::endl;
    }

    void send_message(const std::ostream &oss_)
    {
        std::lock_guard<std::mutex> lock(message_mutex());
        std::cout << static_cast<const std::ostringstream &>(oss_).str()
                  << std::endl;
    }

    void send_error(const std::string &message_)
    {

        std::lock_guard<std::mutex> lock(message_mutex());
        std::cerr << message_ << std::endl;
    }

    void send_error(const std::ostream &oss_)
    {
        std::lock_guard<std::mutex> lock(message_mutex());
        std::cerr << static_cast<const std::ostringstream &>(oss_).str()
                  << std::endl;
    }
} // namespace probare
