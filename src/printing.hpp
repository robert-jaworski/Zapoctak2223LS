#pragma once

#include <iostream>
#include <iomanip>
#include "matrix.hpp"
#include "fixed_matrix.hpp"
#include "number_types.hpp"

namespace matrices {

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const matrix<T>& m) {
        for (int i = 0; i < m.rows(); i++) {
            os << "| " << std::setw(3) << m.element(i, 0);
            for (int j = 1; j < m.cols(); j++) {
                os << ' ' << std::setw(3) << m.element(i, j);
            }
            os << " |\n";
        }
        return os;
    }

    template <typename T, int ROWS, int COLS>
    std::ostream& operator<<(std::ostream& os, const fixed_matrix<T, ROWS, COLS>& m) {
        for (int i = 0; i < m.rows(); i++) {
            os << "| " << std::setw(3) << m.element(i, 0);
            for (int j = 1; j < m.cols(); j++) {
                os << ' ' << std::setw(3) << m.element(i, j);
            }
            os << " |\n";
        }
        return os;
    }

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const finite_field<T>& x) {
        return os << x.value();
    }

    template <typename T>
    inline std::istream& operator>>(std::istream& os, const finite_field<T>& x) {
        T val;
        os >> val;
        x = val;
        return os;
    }

    template <typename T, T P>
    inline std::ostream& operator<<(std::ostream& os, const finite_field_template<T, P>& x) {
        return os << x.value();
    }

    template <typename T, T P>
    inline std::istream& operator>>(std::istream& os, const finite_field_template<T, P>& x) {
        T val;
        os >> val;
        x = val;
        return os;
    }

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const fraction<T>& x) {
        return os << x.numerator() << "/" << x.denominator();
    }

}
