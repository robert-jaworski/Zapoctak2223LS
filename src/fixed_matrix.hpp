#pragma once

#include <vector>
#include <array>
#include "assert.hpp"
#include "matrix.hpp"

namespace matrices {

    namespace helper {
        template <typename T, int ROWS, int COLS> class fixed_matrix_indexer;
        template <typename T, int ROWS, int COLS> class const_fixed_matrix_indexer;
    }

    template <typename T, int ROWS, int COLS>
    class fixed_matrix {
        std::array<T, ROWS * COLS> elements;

        inline const T& get_elem(int row, int col) const {
            return elements[row * COLS + col];
        }

        inline T& get_elem(int row, int col) {
            return const_cast<T&>(const_cast<const fixed_matrix<T, ROWS, COLS>*>(this)->get_elem(row, col));
        }

        inline void check_index(int row, int col) const {
            do_assert(0 <= row && row < ROWS && 0 <= col && col < COLS, "Invalid element index");
        }

        inline void check_dimen(const std::vector<T>& v) const {
            do_assert(elements.size() == v.size(), "Wrong vector size for matrix assignment");
        }

        inline void check_dimen(const std::vector<std::vector<T>>& v) const {
            do_assert(v.size() == ROWS, "Wrong vector size for matrix assignment - rows count");
            for (auto& row : v) {
                do_assert(row.size() == COLS, "Wrong vector size for matrix assignment - columns count");
            }
        }

    public:
        fixed_matrix() = default;

        fixed_matrix(const T& fill) {
            elements.fill(fill);
        }

        inline fixed_matrix(const std::vector<T>& elems) {
            check_dimen(elems);
            for (int i = 0; i < elements.size(); i++) {
                elements[i] = elems[i];
            }
        }

        inline fixed_matrix(const std::vector<std::vector<T>>& elems) {
            *this = elems;
        }

        inline operator matrix<T>() const {
            return matrix<T>(ROWS, COLS, elements);
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
            return const_cast<T&>(const_cast<const fixed_matrix<T, ROWS, COLS>*>(this)->element(row, col));
        }

        inline helper::fixed_matrix_indexer<T, ROWS, COLS> operator[](int row) {
            return helper::fixed_matrix_indexer<T, ROWS, COLS>(*this, row);
        }

        inline helper::const_fixed_matrix_indexer<T, ROWS, COLS> operator[](int row) const {
            return helper::fixed_matrix_indexer<T, ROWS, COLS>(*this, row);
        }

        fixed_matrix<T, ROWS, COLS>& operator=(const fixed_matrix<T, ROWS, COLS>& rhs) {
            elements = rhs.elements;
            return *this;
        }

        fixed_matrix<T, ROWS, COLS>& operator=(const std::vector<T>& rhs) {
            check_dimen(rhs);
            elements = rhs;
            return *this;
        }

        fixed_matrix<T, ROWS, COLS>& operator=(const std::vector<std::vector<T>>& rhs) {
            check_dimen(rhs);
            for (int i = 0, k = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++, k++) {
                    elements[k] = rhs[i][j];
                }
            }
            return *this;
        }

        bool operator==(const fixed_matrix<T, ROWS, COLS>& rhs) const {
            return elements == rhs.elements;
        }

        fixed_matrix<T, ROWS, COLS> operator+(const fixed_matrix<T, ROWS, COLS>& rhs) const {
            fixed_matrix<T, ROWS, COLS> out = *this;
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] + rhs.elements[i];
            }
            return out;
        }

        fixed_matrix<T, ROWS, COLS>& operator+=(const fixed_matrix<T, ROWS, COLS>& rhs) {
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] += rhs.elements[i];
            }
            return *this;
        }

        fixed_matrix<T, ROWS, COLS> operator-() const {
            fixed_matrix<T, ROWS, COLS> out = *this;
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = -elements[i];
            }
            return out;
        }

        fixed_matrix<T, ROWS, COLS> operator-(const fixed_matrix<T, ROWS, COLS>& rhs) const {
            fixed_matrix<T, ROWS, COLS> out = *this;
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] - rhs.elements[i];
            }
            return out;
        }

        fixed_matrix<T, ROWS, COLS>& operator-=(const fixed_matrix<T, ROWS, COLS>& rhs) {
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] -= rhs.elements[i];
            }
            return *this;
        }

        fixed_matrix<T, ROWS, COLS> operator*(const T& rhs) const {
            fixed_matrix<T, ROWS, COLS> out;
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] * rhs;
            }
            return out;
        }

        fixed_matrix<T, ROWS, COLS>& operator*=(const T& rhs) {
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] *= rhs;
            }
            return *this;
        }

        template <int rhsCOLS>
        fixed_matrix<T, ROWS, rhsCOLS> operator*(const fixed_matrix<T, COLS, rhsCOLS>& rhs) const {
            fixed_matrix<T, ROWS, rhsCOLS> out;
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < rhsCOLS; j++) {
                    out.get_elem(i, j) -= out.get_elem(i, j);
                    for (int k = 0; k < COLS; k++) {
                        out.get_elem(i, j) += get_elem(i, k) * rhs.get_elem(k, j);
                    }
                }
            }
            return out;
        }

        fixed_matrix<T, ROWS, COLS>& operator*=(const fixed_matrix<T, COLS, COLS>& rhs) {
            for (int i = 0; i < ROWS; i++) {
                std::vector<T> temp(COLS);
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

        fixed_matrix<T, ROWS, COLS>& multiply_from_left(const fixed_matrix<T, ROWS, ROWS>& rhs) {
            for (int j = 0; j < COLS; j++) {
                std::vector<T> temp(ROWS);
                for (int i = 0; i < ROWS; i++) {
                    for (int k = 0; k < COLS; k++) {
                        temp[i] += get_elem(i, k) * rhs.get_elem(k, j);
                    }
                }
                for (int i = 0; i < ROWS; i++) {
                    get_elem(i, j) = temp[i];
                }
            }
            return *this;
        }
    };

    template <typename T, int ROWS, int COLS>
    fixed_matrix<T, ROWS, COLS> operator*(const T& lhs, const fixed_matrix<T, ROWS, COLS>& rhs) {
        return rhs * lhs;
    }

    namespace helper {

        template <typename T, int ROWS, int COLS>
        class const_fixed_matrix_indexer {
            const fixed_matrix<T, ROWS, COLS>& currentMatrix;
            const int currentRow;
        
        public:
            inline const_fixed_matrix_indexer(const fixed_matrix<T, ROWS, COLS>& m, int row) : currentMatrix(m), currentRow(row) { }

            inline const T& operator[](int col) const {
                return currentMatrix.element(currentRow, col);
            }
        };

        template <typename T, int ROWS, int COLS>
        class fixed_matrix_indexer {
            fixed_matrix<T, ROWS, COLS>& currentMatrix;
            const int currentRow;
        
        public:
            inline fixed_matrix_indexer(fixed_matrix<T, ROWS, COLS>& m, int row) : currentMatrix(m), currentRow(row) { }

            inline T& operator[](int col) const {
                return currentMatrix.element(currentRow, col);
            }
        };

    }

}
