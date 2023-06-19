#include "assert.hpp"

namespace matrices {

    const char* exceptions::assert_error::what() const noexcept {
        return message.c_str();
    }

};
