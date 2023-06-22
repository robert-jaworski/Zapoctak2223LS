#include "numbers.hpp"

namespace number_utils {

    template<>
    unsigned int get_random<unsigned int>(const unsigned int& max) {
        return (unsigned int)get_random<unsigned long long>(max);
    }

    template<>
    signed int get_random<signed int>(const signed int& max) {
        return (signed int)get_random<unsigned long long>(max);
    }

    template<>
    unsigned short get_random<unsigned short>(const unsigned short& max) {
        return (unsigned short)get_random<unsigned long long>(max);
    }

    template<>
    signed short get_random<signed short>(const signed short& max) {
        return (signed short)get_random<unsigned long long>(max);
    }

}
