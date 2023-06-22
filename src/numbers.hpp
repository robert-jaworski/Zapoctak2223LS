#pragma once

#include "assert.h"
#include <limits>
#include <vector>
#include <tuple>
#include <stdexcept>

namespace number_utils {

    template <typename T>
    struct standard_numbers {
        static constexpr inline T zero() {
            return 0;
        }

        static constexpr inline T one() {
            return 1;
        }

        static constexpr inline T minus_one() {
            return -1;
        }

        static constexpr inline T zero(const T& sample) {
            return 0;
        }

        static constexpr inline T one(const T& sample) {
            return 1;
        }

        static constexpr inline T minus_one(const T& sample) {
            return -1;
        }
    };

    template <typename T>
    inline T get_zero() {
        return standard_numbers<T>::zero();
    }

    template <typename T>
    inline T get_one() {
        return standard_numbers<T>::one();
    }

    template <typename T>
    inline T get_minus_one() {
        return standard_numbers<T>::minus_one();
    }

    template <typename T>
    inline T get_zero(const T& sample) {
        return standard_numbers<T>::zero(sample);
    }

    template <typename T>
    inline T get_one(const T& sample) {
        return standard_numbers<T>::one(sample);
    }

    template <typename T>
    inline T get_minus_one(const T& sample) {
        return standard_numbers<T>::minus_one(sample);
    }

    template<typename T>
    constexpr inline bool will_add_overflow(const T& a, const T& b) {
        if (a >= 0) {
            T a1 = std::numeric_limits<T>::max() - a;
            return b > a1;
        }
        if (b < 0) {
            T b1 = b - std::numeric_limits<T>::min();
            return -a > b1;
        }
        return false;
    }

    template<typename T>
    constexpr inline bool can_mul_overflow(const T& a, const T& b) {
        return true;
    }

    template<>
    constexpr inline bool can_mul_overflow<unsigned long long>(const unsigned long long& a, const unsigned long long& b) {
        return a > std::numeric_limits<unsigned int>::max() || b > std::numeric_limits<unsigned int>::max();
    }

    template<typename T>
    constexpr inline bool is_power_of_2(const T& x) {
        return (x & (x - 1)) == 0;
    }

    template<typename T>
    constexpr T get_log(T N, const T& base) {
        if (N == 0) throw std::logic_error("Cannot get the logarithm of 0");
        T x = 0;
        while (N != 0) {
            N /= base;
            ++x;
        }
        return --x;
    }

    template<typename T>
    constexpr bool is_power_of(T N, const T& base) {
        if (N == 0) return false;
        while (N != 0) {
            T x = N % base;
            if (x != 0) return false;
            N /= base;
        }
        return true;
    }

    template<typename T>
    constexpr T p_valuation(T N, const T& p) {
        if (N == 0) throw std::logic_error("Cannot get the p-valuation of 0");
        T val = 0;
        while (N != 0) {
            T x = N % p;
            if (x != 0) return val;
            N /= p;
            ++val;
        }
        return val;
    }

    template<typename T>
    T get_random(const T& max) {
        T base = RAND_MAX;
        ++base;
        T log = get_log(max, base);
        log += 2;
        T curr = 0;
        while (log != 0) {
            curr *= base;
            curr += rand();
            --log;
        }
        return curr % max;
    }

    template<>
    unsigned int get_random<unsigned int>(const unsigned int& max);

    template<>
    signed int get_random<signed int>(const signed int& max);

    template<>
    unsigned short get_random<unsigned short>(const unsigned short& max);

    template<>
    signed short get_random<signed short>(const signed short& max);

    template<typename T>
    constexpr inline T power(T x, T e) {
        if (e == 0) return T{1};
        T res = power(x, e / 2);
        res *= res;
        if (e % 2 == 1) res *= x;
        return res;
    }

    template<typename T>
    constexpr inline T gcd(T a, T b) {
        if (a < 0) a *= -1;
        if (b < 0) b *= -1;
        if (a > b) {
            std::swap(a, b);
        }
        while (a > 0) {
            std::tie(a, b) = std::make_pair(b % a, a);
        }
        return b;
    }

    template<typename T>
    constexpr inline T lcm(T a, T b) {
        return (a / gcd(a, b)) * b;
    }

    template<typename T>
    constexpr inline std::pair<T, std::pair<T, T>> gcd_extended(T a, T b) {
        T q{}, r{}, Aa{1}, Ab{0}, Ba{0}, Bb{1};
        if (a > b) {
            std::swap(a, b);
            std::swap(Aa, Ba);
            std::swap(Ab, Bb);
        }
        while (a > 0) {
            q = b / a;
            r = b % a;
            std::tie(a, b) = std::make_pair(r, a);
            std::tie(Aa, Ba) = std::make_pair(Ba - q * Aa, Aa);
            std::tie(Ab, Bb) = std::make_pair(Bb - q * Ab, Ab);
        }
        return std::make_pair(b, std::make_pair(Ba, Bb));
    }

    /*
    template<typename T>
    class IntModN {
        const T N;
        T X;

        inline void fix() {
            X %= N;
        }

        inline void check(const IntModN<T>& i) const {
            if (i.N != N) throw std::logic_error("Non-matching mod base");
        }

    public:
        inline IntModN(const T& n, const T& x = 0) : N(n), X(x) {
            fix();
        }

        inline const IntModN<T>& operator=(const T& rhs) {
            X = rhs;
            fix();
            return *this;
        }

        inline const IntModN<T>& operator=(const IntModN<T>& rhs) {
            check(rhs);
            X = rhs.X;
            return *this;
        }

        inline bool operator==(const IntModN<T>& rhs) const {
            return X == rhs.X;
        }

        inline bool operator==(const T& rhs) const {
            return X == rhs;
        }

        inline bool operator!=(const IntModN<T>& rhs) const {
            return X != rhs.X;
        }

        inline bool operator!=(const T& rhs) const {
            return X != rhs;
        }

        inline bool operator<(const IntModN<T>& rhs) const {
            return X < rhs.X;
        }

        inline bool operator<(const T& rhs) const {
            return X < rhs;
        }

        inline bool operator<=(const IntModN<T>& rhs) const {
            return X <= rhs.X;
        }

        inline bool operator<=(const T& rhs) const {
            return X <= rhs;
        }

        inline bool operator>(const IntModN<T>& rhs) const {
            return X > rhs.X;
        }

        inline bool operator>(const T& rhs) const {
            return X > rhs;
        }

        inline bool operator>=(const IntModN<T>& rhs) const {
            return X >= rhs.X;
        }

        inline bool operator>=(const T& rhs) const {
            return X >= rhs;
        }

        inline const T& val() const {
            return X;
        }

        inline const T& mod() const {
            return N;
        }

        inline const IntModN<T>& operator++() {
            return operator+=(1);
        }

        inline const IntModN<T>& operator++(int) {
            IntModN<T> tmp = *this;
            operator+=(1);
            return tmp;
        }

        inline const IntModN<T>& operator--() {
            return operator-=(1);
        }

        inline const IntModN<T>& operator--(int) {
            IntModN<T> tmp = *this;
            operator-=(1);
            return tmp;
        }

        inline IntModN<T> operator-() const {
            return { N, N - X };
        }

        inline IntModN<T> operator+(const T& rhs) const {
            if (will_add_overflow(X, rhs)) return -IntModN(N, (N - X) + (N - rhs));
            return { N, X + rhs };
        }

        inline IntModN<T>&& operator+(const IntModN<T>& rhs) const {
            check(rhs);
            return operator+(rhs.X);
        }

        inline IntModN<T>&& operator-(const T& rhs) const {
            return operator+(N - rhs);
        }

        inline IntModN<T>&& operator-(const IntModN<T>& rhs) const {
            check(rhs);
            return operator-(rhs.X);
        }

        inline const IntModN<T>& operator+=(const T& rhs) {
            if (will_add_overflow(X, rhs)) {
                X = (N - X) + (N - rhs);
                fix();
                X = N - X;
                return *this;
            }
            else {
                X += rhs;
                fix();
                return *this;
            }
        }

        inline const IntModN<T>& operator+=(const IntModN<T>& rhs) {
            check(rhs);
            return operator+=(rhs.X);
        }

        inline const IntModN<T>& operator-=(const T& rhs) {
            return operator+=(N - rhs);
        }

        inline const IntModN<T>& operator-=(const IntModN<T>& rhs) {
            check(rhs);
            return operator-=(rhs.X);
        }

        inline IntModN<T> operator*(const T& rhs) const {
            if (!can_mul_overflow(X, rhs)) return { N, X * rhs };
            if (rhs == 0) return { N, 0 };
            if (rhs == 1) return *this;
            IntModN<T> tmp = operator*(rhs / 2);
            tmp += tmp.X;
            if (rhs % 2 == 1) tmp += X;
            return tmp;
        }

        inline IntModN<T>&& operator*(const IntModN<T>& rhs) const {
            check(rhs);
            return operator*(rhs.X);
        }

        inline const IntModN<T>& operator*=(const T& rhs) {
            X = operator*(rhs).X;
            return *this;
        }

        inline const IntModN<T>& operator*=(const IntModN<T>& rhs) {
            check(rhs);
            return operator*=(rhs.X);
        }

        inline IntModN<T> operator^(const T& rhs) const {
            if (rhs == 0) {
                if (X == 0) throw std::logic_error("0^0 is indeterminate");
                return { N, 1 };
            }
            if (rhs == 1) return *this;
            IntModN<T> tmp = operator^(rhs / 2);
            tmp *= tmp.X;
            if (rhs % 2 == 1) tmp *= X;
            return tmp;
        }

        inline IntModN<T>&& operator^(const IntModN<T>& rhs) const {
            check(rhs);
            return operator^(rhs.X);
        }

        inline const IntModN<T>& operator^=(const T& rhs) {
            X = operator^(rhs).X;
            return *this;
        }

        inline const IntModN<T>& operator^=(const IntModN<T>& rhs) {
            check(rhs);
            return operator^=(rhs.X);
        }
    };
    */

    template<typename T>
    class digits_view {
        const std::vector<T>* digits;
        size_t start;
        size_t length;

    public:
        digits_view(const std::vector<T>* d, size_t s, size_t l) {
            assert(d != nullptr && s + l <= d->size());
            digits = d;
            start = s;
            length = l;
        }

        digits_view(const std::vector<T>* d) {
            assert(d != nullptr);
            digits = d;
            start = 0;
            length = d->size();
        }

        digits_view(const std::vector<T>& d) {
            digits = &d;
            start = 0;
            length = d.size();
        }

        digits_view view(size_t s, size_t l) const {
            assert(s + l <= size());
            return digits_view(digits, start + s, l);
        }

        size_t size() const {
            return length;
        }

        const T& at(size_t i) const {
            assert(i < length);
            return digits->at(start + i);
        }

        const T& operator[](size_t i) const {
            return at(i);
        }

        const T& back() const {
            return digits->back();
        }
    };

}
