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

        inline void check_index(int row, int col) const {
            do_assert(0 <= row && row < ROWS && 0 <= col && col < COLS, "Invalid element index");
        }

    public:
        inline matrix(int rows, int columns) : ROWS(rows), COLS(columns) {
            elements.resize(ROWS * COLS);
        }

        inline matrix(int rows, int columns, const T& fill) : ROWS(rows), COLS(columns) {
            elements.resize(ROWS * COLS, fill);
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
            do_assert(dimension() == rhs.dimension(), "Incompatible matrix dimensions");
            elements = rhs.elements;
            return *this;
        }

        matrix<T>& operator=(const std::vector<T>& rhs) {
            do_assert(rhs.size() == elements.size(), "Wrong vector size for matrix assignment");
            elements = rhs;
            return *this;
        }
    };

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
