#pragma once

#include <vector>
#include <array>
#include "assert.hpp"

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

        inline void check_index(int row, int col) const {
            do_assert(0 <= row && row < ROWS && 0 <= col && col < COLS, "Invalid element index");
        }

    public:
        fixed_matrix() = default;

        fixed_matrix(const T& fill) {
            elements.fill(fill);
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
            do_assert(dimension() == rhs.dimension(), "Incompatible matrix dimensions");
            elements = rhs.elements;
            return *this;
        }

        fixed_matrix<T, ROWS, COLS>& operator=(const std::vector<T>& rhs) {
            do_assert(rhs.size() == elements.size(), "Wrong vector size for matrix assignment");
            elements = rhs;
            return *this;
        }
    };

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
