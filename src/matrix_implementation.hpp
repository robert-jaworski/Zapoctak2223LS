#pragma once

#include "numbers.hpp"

namespace matrices {

    template <typename T, typename M>
    struct standard_matrices {
        M identity(int size) {
            return M::identity(size);
        }

        M identity(int size, const T& sample) {
            return M::identity(size, sample);
        }
    };

    namespace helper {

        template <typename T, typename M>
        struct matrix_impl {

            static inline void multiply(M& out, const M& lhs, const M& rhs) {
                for (int i = 0; i < lhs.rows(); i++) {
                    for (int j = 0; j < rhs.cols(); j++) {
                        for (int k = 0; k < rhs.cols(); k++) {
                            out.get_elem(i, j) += lhs.get_elem(i, k) * rhs.get_elem(k, j);
                        }
                    }
                }
            }

            static inline void multiply_from_right(M& lhs, const M& rhs) {
                rhs.assert_square();
                if (&lhs == &rhs) {
                    lhs = lhs * rhs;
                    return;
                }

                for (int i = 0; i < lhs.rows(); i++) {
                    std::vector<T> temp(lhs.cols(), number_utils::get_zero<T>(lhs.elements[0]));
                    for (int j = 0; j < lhs.cols(); j++) {
                        for (int k = 0; k < lhs.cols(); k++) {
                            temp[j] += lhs.get_elem(i, k) * rhs.get_elem(k, j);
                        }
                    }
                    for (int j = 0; j < lhs.cols(); j++) {
                        lhs.get_elem(i, j) = temp[j];
                    }
                }
            }

            static inline void multiply_from_left(const M& lhs, M& rhs) {
                lhs.assert_square();
                if (&rhs == &lhs) {
                    rhs = lhs * rhs;
                    return;
                }

                for (int j = 0; j < rhs.cols(); j++) {
                    std::vector<T> temp(rhs.rows(), number_utils::get_zero<T>(rhs.elements[0]));
                    for (int i = 0; i < rhs.rows(); i++) {
                        for (int k = 0; k < rhs.cols(); k++) {
                            temp[i] += lhs.get_elem(i, k) * rhs.get_elem(k, j);
                        }
                    }
                    for (int i = 0; i < rhs.rows(); i++) {
                        rhs.get_elem(i, j) = temp[i];
                    }
                }
            }

            static inline M power(const M& lhs, int power) {
                lhs.assert_square();
                if (power == 0)
                    return M::identity(lhs.rows(), lhs.elements[0]);
                if (power == 1)
                    return lhs;
                
                if (power < 0) {
                    std::pair<M, bool> RREF_inverse = lhs.compute_inverse_RREF();
                    do_assert(RREF_inverse.second, "Cannot compute the inverse matrix - singular");
                    return RREF_inverse.first ^ -power;
                }
                M half = lhs ^ (power / 2);
                half *= half;
                if (power % 2)
                    half *= lhs;
                return half;
            }

            static inline std::pair<int, T> compute_REF_rank_det(M& m) {
                int i, p, swaps = 0;
                for (i = 0, p = 0; i < m.rows() && p < m.cols(); i++, p++) {
                    while (m.get_elem(i, p) == number_utils::get_zero<T>(m.elements[0])) {
                        int j;
                        for (j = i + 1; j < m.rows(); j++) {
                            if (m.get_elem(j, p) != number_utils::get_zero<T>(m.elements[0])) {
                                swaps++;
                                for (int k = p; k < m.cols(); k++) {
                                    std::swap(m.get_elem(i, k), m.get_elem(j, k));
                                }
                                break;
                            }
                        }
                        if (j >= m.rows())
                            p++;
                        if (p >= m.cols()) {
                            T det = m.elements[0];
                            for (int i = 1; i < m.rows() && i < m.cols(); i++) {
                                det *= m.get_elem(i, i);
                            }
                            return std::make_pair(i, swaps % 2 ? -det : det);
                        }
                    }
                    for (int j = i + 1; j < m.rows(); j++) {
                        if (m.get_elem(j, p) != number_utils::get_zero<T>(m.elements[0])) {
                            T mult = m.get_elem(j, p) / m.get_elem(i, p);
                            for (int k = p; k < m.cols(); k++) {
                                m.get_elem(j, k) -= mult * m.get_elem(i, k);
                            }
                        }
                    }
                }
                T det = m.elements[0];
                for (int i = 1; i < m.rows() && i < m.cols(); i++) {
                    det *= m.get_elem(i, i);
                }
                return std::make_pair(i, swaps % 2 ? -det : det);
            }

            static inline int compute_RREF_and_rank(M& m) {
                int i, p;
                for (i = 0, p = 0; i < m.rows() && p < m.cols(); i++, p++) {
                    while (m.get_elem(i, p) == number_utils::get_zero<T>(m.elements[0])) {
                        int j;
                        for (j = i + 1; j < m.rows(); j++) {
                            if (m.get_elem(j, p) != number_utils::get_zero<T>(m.elements[0])) {
                                for (int k = p; k < m.cols(); k++) {
                                    std::swap(m.get_elem(i, k), m.get_elem(j, k));
                                }
                                break;
                            }
                        }
                        if (j >= m.rows())
                            p++;
                        if (p >= m.cols())
                            return i;
                    }
                    for (int j = 0; j < m.rows(); j++) {
                        if (j != i && m.get_elem(j, p) != number_utils::get_zero<T>(m.elements[0])) {
                            T mult = m.get_elem(j, p) / m.get_elem(i, p);
                            for (int k = p; k < m.cols(); k++) {
                                m.get_elem(j, k) -= mult * m.get_elem(i, k);
                            }
                        }
                    }
                    for (int k = p + 1; k < m.cols(); k++) {
                        m.get_elem(i, k) /= m.get_elem(i, p);
                    }
                    m.get_elem(i, p) /= m.get_elem(i, p);
                }
                return i;
            }

            static inline std::pair<M, bool> compute_inverse_RREF(const M& m) {
                m.assert_square();
                M copy = m, inverse = M::identity(m.rows(), m.elements[0]);
                int i, p;
                for (i = 0, p = 0; i < m.rows() && p < m.cols(); i++, p++) {
                    while (copy.get_elem(i, p) == number_utils::get_zero<T>(m.elements[0])) {
                        int j;
                        for (j = i + 1; j < m.rows(); j++) {
                            if (copy.get_elem(j, p) != number_utils::get_zero<T>(m.elements[0])) {
                                for (int k = p; k < m.cols(); k++) {
                                    std::swap(copy.get_elem(i, k), copy.get_elem(j, k));
                                    std::swap(inverse.get_elem(i, k), inverse.get_elem(j, k));
                                }
                                break;
                            }
                        }
                        if (j >= m.rows())
                            p++;
                        if (p >= m.cols())
                            return std::make_pair(inverse, false);
                    }
                    for (int j = 0; j < m.rows(); j++) {
                        if (j != i && copy.get_elem(j, p) != number_utils::get_zero<T>(m.elements[0])) {
                            T mult = copy.get_elem(j, p) / copy.get_elem(i, p);
                            for (int k = p; k < m.cols(); k++) {
                                copy.get_elem(j, k) -= mult * copy.get_elem(i, k);
                                inverse.get_elem(j, k) -= mult * inverse.get_elem(i, k);
                            }
                        }
                    }
                    for (int k = p + 1; k < m.cols(); k++) {
                        copy.get_elem(i, k) /= copy.get_elem(i, p);
                        inverse.get_elem(i, k) /= inverse.get_elem(i, p);
                    }
                    copy.get_elem(i, p) /= copy.get_elem(i, p);
                    inverse.get_elem(i, p) /= inverse.get_elem(i, p);
                }
                return std::make_pair(inverse, i == m.rows());
            }

        };

    }

}
