#pragma once

#include <vector>
#include <array>
#include "assert.hpp"
#include "dynamic_matrix.hpp"

namespace matrices {

    namespace helper {
        template <typename T, int ROWS, int COLS> class matrix_indexer;
        template <typename T, int ROWS, int COLS> class const_matrix_indexer;
    }

    template <typename T, int ROWS, int COLS>
    class matrix;

    template <typename T, int SIZE>
    matrix<T, SIZE, SIZE> identity_matrix();

    template <typename T, int SIZE>
    matrix<T, SIZE, SIZE> identity_matrix(const T& sample);

    template <typename T, int ROWS, int COLS>
    class matrix {
        static_assert(ROWS > 0 && COLS > 0, "Matrix size must be positive");
        std::array<T, ROWS * COLS> elements;

        using impl = helper::matrix_impl<T, matrix<T, ROWS, COLS>>;
        friend impl;

        inline const T& get_elem(int row, int col) const {
            return elements[row * COLS + col];
        }

        inline T& get_elem(int row, int col) {
            return const_cast<T&>(const_cast<const matrix<T, ROWS, COLS>*>(this)->get_elem(row, col));
        }

        static inline void check_index(int row, int col) {
            do_assert(0 <= row && row < ROWS && 0 <= col && col < COLS, "Invalid element index");
        }

        static inline void check_dimen(const std::initializer_list<T>& v) {
            do_assert(ROWS * COLS == v.size(), "Wrong vector size for matrix assignment");
        }

        static inline void check_dimen(const std::initializer_list<std::initializer_list<T>>& v) {
            do_assert(v.size() == ROWS, "Wrong vector size for matrix assignment - rows count");
            for (auto& row : v) {
                do_assert(row.size() == COLS, "Wrong vector size for matrix assignment - columns count");
            }
        }

        static inline void assert_square(int size = ROWS) {
            static_assert(ROWS == COLS, "Must be a square matrix");
            do_assert(size == COLS, "Wrong matrix size");
        }

    public:
        matrix() = default;

        matrix(const T& fill) {
            elements.fill(fill);
        }

        inline matrix(const std::initializer_list<T>& elems) {
            *this = elems;
        }

        inline matrix(const std::initializer_list<std::initializer_list<T>>& elems) {
            *this = elems;
        }

        inline operator dynamic_matrix<T>() const {
            return dynamic_matrix<T>(ROWS, COLS, elements);
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
            return const_cast<T&>(const_cast<const matrix<T, ROWS, COLS>*>(this)->element(row, col));
        }

        inline helper::matrix_indexer<T, ROWS, COLS> operator[](int row) {
            return helper::matrix_indexer<T, ROWS, COLS>(*this, row);
        }

        inline helper::const_matrix_indexer<T, ROWS, COLS> operator[](int row) const {
            return helper::matrix_indexer<T, ROWS, COLS>(*this, row);
        }

        static matrix<T, ROWS, COLS> identity(int size) {
            assert_square(size);
            matrix<T, ROWS, COLS> out(number_utils::get_zero<T>());
            for (int i = 0; i < size; i++) {
                out[i][i] = number_utils::get_one<T>();
            }
            return out;
        }

        static matrix<T, ROWS, COLS> identity(int size, const T& sample) {
            assert_square(size);
            matrix<T, ROWS, COLS> out(number_utils::get_zero<T>(sample));
            for (int i = 0; i < size; i++) {
                out[i][i] = number_utils::get_one<T>(sample);
            }
            return out;
        }

        matrix<T, ROWS, COLS>& operator=(const matrix<T, ROWS, COLS>& rhs) {
            elements = rhs.elements;
            return *this;
        }

        matrix<T, ROWS, COLS>& operator=(const std::initializer_list<T>& rhs) {
            check_dimen(rhs);
            const T *it = rhs.begin();
            for (int i = 0; i < elements.size(); i++, it++) {
                elements[i] = *it;
            }
            return *this;
        }

        matrix<T, ROWS, COLS>& operator=(const std::initializer_list<std::initializer_list<T>>& rhs) {
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

        bool operator==(const matrix<T, ROWS, COLS>& rhs) const {
            return elements == rhs.elements;
        }

        bool operator!=(const matrix<T, ROWS, COLS>& rhs) const {
            return elements != rhs.elements;
        }

        matrix<T, ROWS, COLS> operator+(const matrix<T, ROWS, COLS>& rhs) const {
            matrix<T, ROWS, COLS> out = *this;
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] + rhs.elements[i];
            }
            return out;
        }

        matrix<T, ROWS, COLS>& operator+=(const matrix<T, ROWS, COLS>& rhs) {
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] += rhs.elements[i];
            }
            return *this;
        }

        matrix<T, ROWS, COLS> operator-() const {
            matrix<T, ROWS, COLS> out = *this;
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = -elements[i];
            }
            return out;
        }

        matrix<T, ROWS, COLS> operator-(const matrix<T, ROWS, COLS>& rhs) const {
            matrix<T, ROWS, COLS> out = *this;
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] - rhs.elements[i];
            }
            return out;
        }

        matrix<T, ROWS, COLS>& operator-=(const matrix<T, ROWS, COLS>& rhs) {
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] -= rhs.elements[i];
            }
            return *this;
        }

        matrix<T, ROWS, COLS> operator*(const T& rhs) const {
            matrix<T, ROWS, COLS> out;
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] * rhs;
            }
            return out;
        }

        matrix<T, ROWS, COLS>& operator*=(const T& rhs) {
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] *= rhs;
            }
            return *this;
        }

        template <int rhsCOLS>
        matrix<T, ROWS, rhsCOLS> operator*(const matrix<T, COLS, rhsCOLS>& rhs) const {
            matrix<T, ROWS, rhsCOLS> out;
            impl::multiply(out, *this, rhs);
            return out;
        }

        matrix<T, ROWS, COLS>& operator*=(const matrix<T, COLS, COLS>& rhs) {
            impl::multiply_from_right(*this, rhs);
            return *this;
        }

        matrix<T, ROWS, COLS>& multiply_from_left(const matrix<T, ROWS, ROWS>& lhs) {
            impl::multiply_from_left(lhs, *this);
            return *this;
        }

        matrix<T, COLS, ROWS>& transpose_self() {
            assert_square();
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < i; j++) {
                    std::swap(get_elem(i, j), get_elem(j, i));
                }
            }
            return *this;
        }

        matrix<T, COLS, ROWS> transpose() const {
            matrix<T, COLS, ROWS> out;
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    out.element(j, i) = get_elem(i, j);
                }
            }
            return out;
        }

        std::pair<int, T> compute_REF_rank_det() {
            return impl::compute_REF_rank_det(*this);
        }

        inline matrix<T, ROWS, COLS>& do_REF() {
            compute_REF_rank_det();
            return *this;
        }

        inline matrix<T, ROWS, COLS> get_REF() const {
            matrix<T, ROWS, COLS> out = *this;
            return out.do_REF();
        }

        inline int compute_rank() {
            matrix<T, ROWS, COLS> copy = *this;
            return copy.compute_REF_rank_det().first;
        }

        int compute_RREF_and_rank() {
            return impl::compute_RREF_and_rank(*this);
        }
        
        inline matrix<T, ROWS, COLS>& do_RREF() {
            compute_RREF_and_rank();
            return *this;
        }

        inline matrix<T, ROWS, COLS> get_RREF() const {
            matrix<T, ROWS, COLS> out = *this;
            return out.do_RREF();
        }

        inline std::pair<matrix<T, ROWS, COLS>, bool> compute_inverse_RREF() const {
            return impl::compute_inverse_RREF(*this);
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
            matrix<T, ROWS, COLS> copy = *this;
            return copy.compute_REF_rank_det().second;
        }
    };

    template <typename T, int SIZE>
    matrix<T, SIZE, SIZE> identity_matrix() {
        matrix<T, SIZE, SIZE> out(number_utils::get_zero<T>());
        for (int i = 0; i < SIZE; i++) {
            out[i][i] = number_utils::get_one<T>();
        }
        return out;
    }

    template <typename T, int SIZE>
    matrix<T, SIZE, SIZE> identity_matrix(const T& sample) {
        matrix<T, SIZE, SIZE> out(number_utils::get_zero<T>(sample));
        for (int i = 0; i < SIZE; i++) {
            out[i][i] = number_utils::get_one<T>(sample);
        }
        return out;
    }

    template <typename T, int ROWS, int COLS>
    inline matrix<T, ROWS, COLS> operator*(const T& lhs, const matrix<T, ROWS, COLS>& rhs) {
        return rhs * lhs;
    }

    template <typename T, int SIZE>
    matrix<T, SIZE, SIZE> operator^(const matrix<T, SIZE, SIZE>& lhs, int power) {
        return helper::matrix_impl<T, matrix<T, SIZE, SIZE>>::power(lhs, power);
    }

    template <typename T, int SIZE>
    inline matrix<T, SIZE, SIZE> operator^=(const matrix<T, SIZE, SIZE>& lhs, int power) {
        lhs = lhs ^ power;
        return lhs;
    }

    namespace helper {

        template <typename T, int ROWS, int COLS>
        class const_matrix_indexer {
            const matrix<T, ROWS, COLS>& currentMatrix;
            const int currentRow;
        
        public:
            inline const_matrix_indexer(const matrix<T, ROWS, COLS>& m, int row) : currentMatrix(m), currentRow(row) { }

            inline const T& operator[](int col) const {
                return currentMatrix.element(currentRow, col);
            }
        };

        template <typename T, int ROWS, int COLS>
        class matrix_indexer {
            matrix<T, ROWS, COLS>& currentMatrix;
            const int currentRow;
        
        public:
            inline matrix_indexer(matrix<T, ROWS, COLS>& m, int row) : currentMatrix(m), currentRow(row) { }

            inline T& operator[](int col) const {
                return currentMatrix.element(currentRow, col);
            }
        };

    }

}
