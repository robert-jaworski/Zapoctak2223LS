#pragma once

#include <stdexcept>

namespace matrices {

    namespace exceptions {

        class assert_error : public std::exception {
            std::string message;

        public:
            inline assert_error(const char* msg) {
                message = msg;
            }

            inline assert_error(const std::string& msg) {
                message = msg;
            }

            const char* what() const noexcept;

            inline const std::string& msg() const {
                return message;
            }
        };

    }

    inline void do_assert(bool expr, const char* msg = "Assertion failed") {
        if (!expr)
            throw exceptions::assert_error(msg);
    }

    inline void do_assert(bool expr, const std::string& msg) {
        do_assert(expr, msg.c_str());
    }
}
