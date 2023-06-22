#pragma once

#include <vector>
#include "assert.hpp"
#include "numbers.hpp"

namespace matrices {

    namespace helper {
        template <typename T> class matrix_indexer;
        template <typename T> class const_matrix_indexer;
    }

    template <typename T>
    class dynamic_matrix {
        const int ROWS, COLS;
        std::vector<T> elements;

        inline const T& get_elem(int row, int col) const {
            return elements[row * COLS + col];
        }

        inline T& get_elem(int row, int col) {
            return const_cast<T&>(const_cast<const dynamic_matrix<T>*>(this)->get_elem(row, col));
        }

        inline void check_index(int row, int col) const {
            do_assert(0 <= row && row < ROWS && 0 <= col && col < COLS, "Invalid element index");
        }

        inline void check_dimen(const dynamic_matrix<T>& m) const {
            do_assert(dimension() == m.dimension(), "Incompatible matrix dimensions");
        }

        inline void check_dimen(const std::initializer_list<T>& v) const {
            do_assert(ROWS * COLS == v.size(), "Wrong vector size for matrix assignment");
        }

        inline void check_dimen(const std::initializer_list<std::initializer_list<T>>& v) const {
            do_assert(v.size() == ROWS, "Wrong vector size for matrix assignment - rows count");
            for (auto& row : v) {
                do_assert(row.size() == COLS, "Wrong vector size for matrix assignment - columns count");
            }
        }

        inline void check_mult_dimen(const dynamic_matrix<T>& m) const {
            do_assert(COLS == m.ROWS, "Incompatible matrix dimensions for multiplication");
        }

        inline void assert_square() const {
            do_assert(ROWS == COLS, "Must be a square matrix");
        }

    public:
        inline dynamic_matrix(int rows, int columns) : ROWS(rows), COLS(columns) {
            do_assert(ROWS > 0 && COLS > 0, "Matrix size must be positive");
            elements.resize(ROWS * COLS);
        }

        inline dynamic_matrix(int rows, int columns, const T& fill) : ROWS(rows), COLS(columns) {
            do_assert(ROWS > 0 && COLS > 0, "Matrix size must be positive");
            elements.resize(ROWS * COLS, fill);
        }

        inline dynamic_matrix(int rows, int columns, const std::initializer_list<T>& elems) : ROWS(rows), COLS(columns) {
            do_assert(ROWS > 0 && COLS > 0, "Matrix size must be positive");
            check_dimen(elems);
            elements = elems;
        }

        inline dynamic_matrix(int rows, int columns, const std::initializer_list<std::initializer_list<T>>& elems) : ROWS(rows), COLS(columns) {
            do_assert(ROWS > 0 && COLS > 0, "Matrix size must be positive");
            elements.resize(ROWS * COLS);
            *this = elems;
        }

        inline std::pair<int, int> dimension() const {
            return std::make_pair(ROWS, COLS);
        }

        inline int rows() const {
            return ROWS;
        }

        inline int cols() const {
            return COLS;
        }

        inline const T& element(int row, int col) const {
            check_index(row, col);
            return get_elem(row, col);
        }

        inline T& element(int row, int col) {
            return const_cast<T&>(const_cast<const dynamic_matrix<T>*>(this)->element(row, col));
        }

        inline helper::matrix_indexer<T> operator[](int row) {
            return helper::matrix_indexer<T>(*this, row);
        }

        inline helper::const_matrix_indexer<T> operator[](int row) const {
            return helper::matrix_indexer<T>(*this, row);
        }

        inline T& operator[](std::pair<int, int> index) {
            return element(index.first, index.second);
        }

        inline const T& operator[](std::pair<int, int> index) const {
            return element(index.first, index.second);
        }

        static dynamic_matrix<T> identity(int size) {
            dynamic_matrix<T> out(size, size, number_utils::get_zero<T>());
            for (int i = 0; i < size; i++) {
                out[i][i] = number_utils::get_one<T>();
            }
            return out;
        }

        static dynamic_matrix<T> identity(int size, const T& sample) {
            dynamic_matrix<T> out(size, size, number_utils::get_zero<T>(sample));
            for (int i = 0; i < size; i++) {
                out[i][i] = number_utils::get_one<T>(sample);
            }
            return out;
        }

        dynamic_matrix<T>& operator=(const dynamic_matrix<T>& rhs) {
            check_dimen(rhs);
            elements = rhs.elements;
            return *this;
        }

        dynamic_matrix<T>& operator=(const std::initializer_list<T>& rhs) {
            check_dimen(rhs);
            elements = rhs;
            return *this;
        }

        dynamic_matrix<T>& operator=(const std::initializer_list<std::initializer_list<T>>& rhs) {
            check_dimen(rhs);
            const std::initializer_list<T> *it = rhs.begin();
            for (int i = 0, k = 0; i < ROWS; i++, it++) {
                const T *it2 = it->begin();
                for (int j = 0; j < COLS; j++, k++, it2++) {
                    elements[k] = *it2;
                }
            }
            return *this;
        }

        bool operator==(const dynamic_matrix<T>& rhs) const {
            return dimension() == rhs.dimension() && elements == rhs.elements;
        }

        dynamic_matrix<T> operator+(const dynamic_matrix<T>& rhs) const {
            check_dimen(rhs);
            dynamic_matrix<T> out(ROWS, COLS);
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] + rhs.elements[i];
            }
            return out;
        }

        dynamic_matrix<T>& operator+=(const dynamic_matrix<T>& rhs) {
            check_dimen(rhs);
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] += rhs.elements[i];
            }
            return *this;
        }

        dynamic_matrix<T> operator-() const {
            dynamic_matrix<T> out(ROWS, COLS);
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = -elements[i];
            }
            return out;
        }

        dynamic_matrix<T> operator-(const dynamic_matrix<T>& rhs) const {
            check_dimen(rhs);
            dynamic_matrix<T> out(ROWS, COLS);
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] - rhs.elements[i];
            }
            return out;
        }

        dynamic_matrix<T>& operator-=(const dynamic_matrix<T>& rhs) {
            check_dimen(rhs);
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] -= rhs.elements[i];
            }
            return *this;
        }

        dynamic_matrix<T> operator*(const T& rhs) const {
            dynamic_matrix<T> out(ROWS, COLS);
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] * rhs;
            }
            return out;
        }

        dynamic_matrix<T>& operator*=(const T& rhs) {
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] *= rhs;
            }
            return *this;
        }

        dynamic_matrix<T> operator*(const dynamic_matrix<T>& rhs) const {
            check_mult_dimen(rhs);
            dynamic_matrix<T> out(ROWS, rhs.COLS, number_utils::get_zero<T>(elements[0]));
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < rhs.COLS; j++) {
                    for (int k = 0; k < COLS; k++) {
                        out.get_elem(i, j) += get_elem(i, k) * rhs.get_elem(k, j);
                    }
                }
            }
            return out;
        }

        dynamic_matrix<T>& operator*=(const dynamic_matrix<T>& rhs) {
            check_mult_dimen(rhs);
            rhs.assert_square();
            if (this == &rhs) {
                *this = *this * rhs;
                return *this;
            }

            for (int i = 0; i < ROWS; i++) {
                std::vector<T> temp(COLS, number_utils::get_zero<T>(elements[0]));
                for (int j = 0; j < COLS; j++) {
                    for (int k = 0; k < COLS; k++) {
                        temp[j] += get_elem(i, k) * rhs.get_elem(k, j);
                    }
                }
                for (int j = 0; j < COLS; j++) {
                    get_elem(i, j) = temp[j];
                }
            }
            return *this;
        }

        dynamic_matrix<T>& multiply_from_left(const dynamic_matrix<T>& lhs) {
            lhs.check_mult_dimen(*this);
            lhs.assert_square();
            if (this == &lhs) {
                *this = lhs * *this;
                return *this;
            }

            for (int j = 0; j < COLS; j++) {
                std::vector<T> temp(ROWS, number_utils::get_zero<T>());
                for (int i = 0; i < ROWS; i++) {
                    for (int k = 0; k < COLS; k++) {
                        temp[i] += get_elem(i, k) * lhs.get_elem(k, j);
                    }
                }
                for (int i = 0; i < ROWS; i++) {
                    get_elem(i, j) = temp[i];
                }
            }
            return *this;
        }

        dynamic_matrix<T> operator^(int power) const {
            assert_square();
            if (power == 0)
                return identity(ROWS, elements[0]);
            if (power == 1)
                return *this;
            
            if (power < 0) {
                std::pair<dynamic_matrix<T>, bool> RREF_inverse = compute_inverse_RREF();
                do_assert(RREF_inverse.second, "Cannot compute the inverse matrix - singular");
                return RREF_inverse.first ^ -power;
            }
            dynamic_matrix<T> half = *this ^ (power / 2);
            half *= half;
            if (power % 2)
                half *= *this;
            return half;
        }

        inline dynamic_matrix<T>& operator^=(int power) {
            *this = *this ^ power;
            return *this;
        }

        dynamic_matrix<T>& transpose_self() {
            assert_square();
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < i; j++) {
                    std::swap(get_elem(i, j), get_elem(j, i));
                }
            }
            return *this;
        }

        dynamic_matrix<T> transpose() const {
            dynamic_matrix<T> out(COLS, ROWS);
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    out.get_elem(j, i) = get_elem(i, j);
                }
            }
            return out;
        }

        std::pair<int, T> compute_REF_rank_det() {
            int i, p, swaps = 0;
            for (i = 0, p = 0; i < ROWS && p < COLS; i++, p++) {
                while (get_elem(i, p) == number_utils::get_zero<T>(elements[0])) {
                    int j;
                    for (j = i + 1; j < ROWS; j++) {
                        if (get_elem(j, p) != number_utils::get_zero<T>(elements[0])) {
                            swaps++;
                            for (int k = p; k < COLS; k++) {
                                std::swap(get_elem(i, k), get_elem(j, k));
                            }
                            break;
                        }
                    }
                    if (j >= ROWS)
                        p++;
                    if (p >= COLS) {
                        T det = elements[0];
                        for (int i = 1; i < ROWS && i < COLS; i++) {
                            det *= get_elem(i, i);
                        }
                        return std::make_pair(i, swaps % 2 ? -det : det);
                    }
                }
                for (int j = i + 1; j < ROWS; j++) {
                    if (get_elem(j, p) != number_utils::get_zero<T>(elements[0])) {
                        T mult = get_elem(j, p) / get_elem(i, p);
                        for (int k = p; k < COLS; k++) {
                            get_elem(j, k) -= mult * get_elem(i, k);
                        }
                    }
                }
            }
            T det = elements[0];
            for (int i = 1; i < ROWS && i < COLS; i++) {
                det *= get_elem(i, i);
            }
            return std::make_pair(i, swaps % 2 ? -det : det);
        }

        inline dynamic_matrix<T>& do_REF() {
            compute_REF_rank_det();
            return *this;
        }

        inline dynamic_matrix<T> get_REF() const {
            dynamic_matrix<T> out = *this;
            return out.do_REF();
        }

        inline int compute_rank() {
            dynamic_matrix<T> copy = *this;
            return copy.compute_REF_rank_det().first;
        }

        int compute_RREF_and_rank() {
            int i, p;
            for (i = 0, p = 0; i < ROWS && p < COLS; i++, p++) {
                while (get_elem(i, p) == number_utils::get_zero<T>(elements[0])) {
                    int j;
                    for (j = i + 1; j < ROWS; j++) {
                        if (get_elem(j, p) != number_utils::get_zero<T>(elements[0])) {
                            for (int k = p; k < COLS; k++) {
                                std::swap(get_elem(i, k), get_elem(j, k));
                            }
                            break;
                        }
                    }
                    if (j >= ROWS)
                        p++;
                    if (p >= COLS)
                        return i;
                }
                for (int j = 0; j < ROWS; j++) {
                    if (j != i && get_elem(j, p) != number_utils::get_zero<T>(elements[0])) {
                        T mult = get_elem(j, p) / get_elem(i, p);
                        for (int k = p; k < COLS; k++) {
                            get_elem(j, k) -= mult * get_elem(i, k);
                        }
                    }
                }
                for (int k = p + 1; k < COLS; k++) {
                    get_elem(i, k) /= get_elem(i, p);
                }
                get_elem(i, p) /= get_elem(i, p);
            }
            return i;
        }
        
        inline dynamic_matrix<T>& do_RREF() {
            compute_RREF_and_rank();
            return *this;
        }

        inline dynamic_matrix<T> get_RREF() const {
            dynamic_matrix<T> out = *this;
            return out.do_RREF();
        }

        std::pair<dynamic_matrix<T>, bool> compute_inverse_RREF() const {
            assert_square();
            dynamic_matrix<T> copy = *this, inverse = identity(ROWS, elements[0]);
            int i, p;
            for (i = 0, p = 0; i < ROWS && p < COLS; i++, p++) {
                while (copy.get_elem(i, p) == number_utils::get_zero<T>(elements[0])) {
                    int j;
                    for (j = i + 1; j < ROWS; j++) {
                        if (copy.get_elem(j, p) != number_utils::get_zero<T>(elements[0])) {
                            for (int k = p; k < COLS; k++) {
                                std::swap(copy.get_elem(i, k), copy.get_elem(j, k));
                                std::swap(inverse.get_elem(i, k), inverse.get_elem(j, k));
                            }
                            break;
                        }
                    }
                    if (j >= ROWS)
                        p++;
                    if (p >= COLS)
                        return std::make_pair(inverse, false);
                }
                for (int j = 0; j < ROWS; j++) {
                    if (j != i && copy.get_elem(j, p) != number_utils::get_zero<T>(elements[0])) {
                        T mult = copy.get_elem(j, p) / copy.get_elem(i, p);
                        for (int k = p; k < COLS; k++) {
                            copy.get_elem(j, k) -= mult * copy.get_elem(i, k);
                            inverse.get_elem(j, k) -= mult * inverse.get_elem(i, k);
                        }
                    }
                }
                for (int k = p + 1; k < COLS; k++) {
                    copy.get_elem(i, k) /= copy.get_elem(i, p);
                    inverse.get_elem(i, k) /= inverse.get_elem(i, p);
                }
                copy.get_elem(i, p) /= copy.get_elem(i, p);
                inverse.get_elem(i, p) /= inverse.get_elem(i, p);
            }
            return std::make_pair(inverse, i == ROWS);
        }

        inline T trace() const {
            assert_square();
            T sum = elements[0];
            for (int i = 1; i < ROWS; i++) {
                sum += get_elem(i, i);
            }
            return sum;
        }

        inline T compute_determinant_REF() const {
            assert_square();
            dynamic_matrix<T> copy = *this;
            return copy.compute_REF_rank_det().second;
        }
    };

    template <typename T>
    inline dynamic_matrix<T> operator*(const T& lhs, const dynamic_matrix<T>& rhs) {
        return rhs * lhs;
    }

    namespace helper {

        template <typename T>
        class const_matrix_indexer {
            const dynamic_matrix<T>& currentMatrix;
            const int currentRow;
        
        public:
            inline const_matrix_indexer(const dynamic_matrix<T>& m, int row) : currentMatrix(m), currentRow(row) { }

            inline const T& operator[](int col) const {
                return currentMatrix.element(currentRow, col);
            }
        };

        template <typename T>
        class matrix_indexer {
            dynamic_matrix<T>& currentMatrix;
            const int currentRow;
        
        public:
            inline matrix_indexer(dynamic_matrix<T>& m, int row) : currentMatrix(m), currentRow(row) { }

            inline T& operator[](int col) const {
                return currentMatrix.element(currentRow, col);
            }
        };

    }

}
