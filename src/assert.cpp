#include "assert.hpp"

namespace matrices {

    const char* exceptions::assert_error::what() const noexcept {
        if (message.size())
            return message.c_str();
        return "Assertion failed";
    }

};
