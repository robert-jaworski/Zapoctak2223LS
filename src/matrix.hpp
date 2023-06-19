#pragma once

#include <vector>
#include "assert.hpp"

namespace matrices {

    namespace helper {
        template <typename T> class matrix_indexer;
        template <typename T> class const_matrix_indexer;
    }

    template <typename T>
    class matrix {
        const int ROWS, COLS;
        std::vector<T> elements;

        inline const T& get_elem(int row, int col) const {
            return elements[row * COLS + col];
        }

        inline T& get_elem(int row, int col) {
            return const_cast<T&>(const_cast<const matrix<T>*>(this)->get_elem(row, col));
        }

        inline void check_index(int row, int col) const {
            do_assert(0 <= row && row < ROWS && 0 <= col && col < COLS, "Invalid element index");
        }

        inline void check_dimen(const matrix<T>& m) const {
            do_assert(dimension() == m.dimension(), "Incompatible matrix dimensions");
        }

        inline void check_dimen(const std::vector<T>& v) const {
            do_assert(ROWS * COLS == v.size(), "Wrong vector size for matrix assignment");
        }

        inline void check_dimen(const std::vector<std::vector<T>>& v) const {
            do_assert(v.size() == ROWS, "Wrong vector size for matrix assignment - rows count");
            for (auto& row : v) {
                do_assert(row.size() == COLS, "Wrong vector size for matrix assignment - columns count");
            }
        }

        inline void check_mult_dimen(const matrix<T>& m) const {
            do_assert(COLS == m.ROWS, "Incompatible matrix dimensions for multiplication");
        }

    public:
        inline matrix(int rows, int columns) : ROWS(rows), COLS(columns) {
            elements.resize(ROWS * COLS);
        }

        inline matrix(int rows, int columns, const T& fill) : ROWS(rows), COLS(columns) {
            elements.resize(ROWS * COLS, fill);
        }

        inline matrix(int rows, int columns, const std::vector<T>& elems) : ROWS(rows), COLS(columns) {
            check_dimen(elems);
            elements = elems;
        }

        inline matrix(int rows, int columns, const std::vector<std::vector<T>>& elems) : ROWS(rows), COLS(columns) {
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
            return const_cast<T&>(const_cast<const matrix<T>*>(this)->element(row, col));
        }

        inline helper::matrix_indexer<T> operator[](int row) {
            return helper::matrix_indexer<T>(*this, row);
        }

        inline helper::const_matrix_indexer<T> operator[](int row) const {
            return helper::matrix_indexer<T>(*this, row);
        }

        matrix<T>& operator=(const matrix<T>& rhs) {
            check_dimen(rhs);
            elements = rhs.elements;
            return *this;
        }

        matrix<T>& operator=(const std::vector<T>& rhs) {
            check_dimen(rhs);
            elements = rhs;
            return *this;
        }

        matrix<T>& operator=(const std::vector<std::vector<T>>& rhs) {
            check_dimen(rhs);
            for (int i = 0, k = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++, k++) {
                    elements[k] = rhs[i][j];
                }
            }
            return *this;
        }

        bool operator==(const matrix<T>& rhs) const {
            return dimension() == rhs.dimension() && elements == rhs.elements;
        }

        matrix<T> operator+(const matrix<T>& rhs) const {
            check_dimen(rhs);
            matrix<T> out(ROWS, COLS);
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] + rhs.elements[i];
            }
            return out;
        }

        matrix<T>& operator+=(const matrix<T>& rhs) {
            check_dimen(rhs);
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] += rhs.elements[i];
            }
            return *this;
        }

        matrix<T> operator-() const {
            matrix<T> out(ROWS, COLS);
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = -elements[i];
            }
            return out;
        }

        matrix<T> operator-(const matrix<T>& rhs) const {
            check_dimen(rhs);
            matrix<T> out(ROWS, COLS);
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] - rhs.elements[i];
            }
            return out;
        }

        matrix<T>& operator-=(const matrix<T>& rhs) {
            check_dimen(rhs);
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] -= rhs.elements[i];
            }
            return *this;
        }

        matrix<T> operator*(const T& rhs) const {
            matrix<T> out(ROWS, COLS);
            for (size_t i = 0; i < elements.size(); i++) {
                out.elements[i] = elements[i] * rhs;
            }
            return out;
        }

        matrix<T>& operator*=(const T& rhs) {
            for (size_t i = 0; i < elements.size(); i++) {
                elements[i] *= rhs;
            }
            return *this;
        }

        matrix<T> operator*(const matrix<T>& rhs) const {
            check_mult_dimen(rhs);
            matrix<T> out(ROWS, rhs.COLS);
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < rhs.COLS; j++) {
                    out.get_elem(i, j) -= out.get_elem(i, j);
                    for (int k = 0; k < COLS; k++) {
                        out.get_elem(i, j) += get_elem(i, k) * rhs.get_elem(k, j);
                    }
                }
            }
            return out;
        }

        matrix<T>& operator*=(const matrix<T>& rhs) {
            check_mult_dimen(rhs);
            do_assert(rhs.COLS == rhs.ROWS, "Must be a square matrix");
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

        matrix<T>& multiply_from_left(const matrix<T>& rhs) {
            rhs.check_mult_dimen(*this);
            do_assert(rhs.COLS == rhs.ROWS, "Must be a square matrix");
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

    template <typename T>
    matrix<T> operator*(const T& lhs, const matrix<T>& rhs) {
        return rhs * lhs;
    }

    namespace helper {

        template <typename T>
        class const_matrix_indexer {
            const matrix<T>& currentMatrix;
            const int currentRow;
        
        public:
            inline const_matrix_indexer(const matrix<T>& m, int row) : currentMatrix(m), currentRow(row) { }

            inline const T& operator[](int col) const {
                return currentMatrix.element(currentRow, col);
            }
        };

        template <typename T>
        class matrix_indexer {
            matrix<T>& currentMatrix;
            const int currentRow;
        
        public:
            inline matrix_indexer(matrix<T>& m, int row) : currentMatrix(m), currentRow(row) { }

            inline T& operator[](int col) const {
                return currentMatrix.element(currentRow, col);
            }
        };

    }

}
