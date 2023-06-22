#pragma once

#include <tuple>
#include "assert.hpp"
#include "numbers.hpp"

namespace matrices {

    template <typename T>
    class finite_field {
        T P;
        T val;

        inline void check_or_set_P(const finite_field<T>& rhs) {
            if (P == 0) {
                P = rhs.P;
            }
            do_assert(P == rhs.P || rhs.P == 0, "Finite field sizes do not match");
        }

        inline void check_P(const finite_field<T>& rhs) const {
            do_assert(P == 0 || P == rhs.P || rhs.P == 0, "Finite field sizes do not match");
        }

        inline void check_P() const {
            do_assert(!(P == 0), "Finite field size not set");
        }

        inline T positive_modulo(const T& x) const {
            if (!(x == 0 || x == 1)) {
                check_P();
                return (P + (x % P)) % P;
            }
            return x;
        }

    public:
        inline finite_field() : P(0), val(0) { }

        inline finite_field(const T& p, const T& value) : P(p) {
            if (P == 0) {
                val = value;
            } else {
                val = positive_modulo(value);
            }
        }

        template <typename U>
        inline finite_field(const finite_field<U>& rhs) : P(rhs.size()), val(rhs.value()) { }

        inline const T& value() const {
            return val;
        }

        inline const T& size() const {
            return P;
        }

        inline finite_field<T>& operator=(const T& rhs) {
            val = positive_modulo(rhs);
            return *this;
        }

        inline finite_field<T>& operator=(const finite_field<T>& rhs) {
            check_or_set_P(rhs);
            if (rhs.P == 0 && !(P == 0)) {
                val = positive_modulo(rhs.val);
            } else {
                val = rhs.val;
            }
            return *this;
        }

        inline bool operator==(const T& rhs) const {
            return val == positive_modulo(rhs);
        }

        inline bool operator==(const finite_field<T>& rhs) const {
            check_P(rhs);
            return val == rhs.val;
        }

        inline bool operator!=(const T& rhs) const {
            return val != positive_modulo(rhs);
        }

        inline bool operator!=(const finite_field<T>& rhs) const {
            check_P(rhs);
            return val != rhs.val;
        }

        inline finite_field<T> operator+(const T& rhs) const {
            return finite_field<T>(P, val + rhs % P);
        }

        inline finite_field<T> operator+(const finite_field<T>& rhs) const {
            check_P(rhs);
            return finite_field<T>(P, val + rhs.val);
        }

        inline finite_field<T>& operator+=(const T& rhs) {
            val = positive_modulo(val + rhs % P);
            return *this;
        }

        inline finite_field<T>& operator+=(const finite_field<T>& rhs) {
            check_or_set_P(rhs);
            val = (val + rhs.val) % P;
            return *this;
        }

        inline finite_field<T> operator-() const {
            return finite_field<T>(P, P - val);
        }

        inline finite_field<T> operator-(const T& rhs) const {
            return finite_field<T>(P, P + val - rhs % P);
        }

        inline finite_field<T> operator-(const finite_field<T>& rhs) const {
            check_P(rhs);
            return finite_field<T>(P, P + val - rhs.val);
        }

        inline finite_field<T>& operator-=(const T& rhs) {
            val = positive_modulo(P + val - rhs % P);
            return *this;
        }

        inline finite_field<T>& operator-=(const finite_field<T>& rhs) {
            check_or_set_P(rhs);
            val = positive_modulo(P + val - rhs.val);
            return *this;
        }

        inline finite_field<T> operator*(const T& rhs) const {
            return finite_field<T>(P, val * positive_modulo(rhs));
        }

        inline finite_field<T> operator*(const finite_field<T>& rhs) const {
            check_P(rhs);
            return finite_field<T>(P, val * rhs.val);
        }

        inline finite_field<T> operator*=(const T& rhs) {
            return *this = val * positive_modulo(rhs);
        }

        inline finite_field<T> operator*=(const finite_field<T>& rhs) {
            check_or_set_P(rhs);
            return *this = val * positive_modulo(rhs.val);
        }

        inline finite_field<T> operator^(int power) const {
            check_P();
            if (power == 0)
                return finite_field<T>(P, 1);
            if (power == 1)
                return *this;
            
            if (power < 0) {
                return (*this ^ (P - number_utils::get_one<T>() - number_utils::get_one<T>())) ^ -power;
            }

            finite_field<T> half = *this ^ (power / 2);
            half *= half;
            if (power % 2)
                half *= *this;
            return half;
        }

        inline finite_field<T>& operator^=(int power) {
            return *this = *this ^ power;
        }

        inline finite_field<T> operator/(const T& rhs) const {
            check_P();
            return *this * (finite_field<T>(P, rhs) ^ -1);
        }

        inline finite_field<T> operator/(const finite_field<T>& rhs) const {
            check_P(rhs);
            return *this * (rhs ^ -1);
        }

        inline finite_field<T> operator/=(const T& rhs) {
            return *this = *this / positive_modulo(rhs);
        }

        inline finite_field<T> operator/=(const finite_field<T>& rhs) {
            check_or_set_P(rhs);
            return *this = *this / positive_modulo(rhs.val);
        }

        inline finite_field<T>& operator++() {
            check_P();
            ++val;
            val = val % P;
            return *this;
        }

        inline finite_field<T> operator++(int) {
            check_P();
            finite_field<T> copy = *this;
            ++*this;
            return copy;
        }

        inline finite_field<T>& operator--() {
            check_P();
            --val;
            val = val % P;
            return *this;
        }

        inline finite_field<T> operator--(int) {
            check_P();
            finite_field<T> copy = *this;
            --*this;
            return copy;
        }
    };

    finite_field<unsigned long long> operator ""_Zp(unsigned long long x) {
        return finite_field<unsigned long long>(0, x);
    }
    
    template <typename T, T P>
    class finite_field_template {
        T val;

        inline T positive_modulo(const T& x) const {
            return (P + (x % P)) % P;
        }

    public:
        inline finite_field_template() {}
        inline finite_field_template(const T& value) : val(positive_modulo(value)) { }

        inline const T& value() const {
            return val;
        }

        static inline const T& size() {
            return P;
        }

        inline finite_field_template<T, P>& operator=(const T& rhs) {
            val = positive_modulo(rhs);
            return *this;
        }

        inline finite_field_template<T, P>& operator=(const finite_field_template<T, P>& rhs) {
            return *this = rhs.val;
        }

        inline operator finite_field<T>() const {
            return finite_field<T>(P, val);
        }

        inline bool operator==(const T& rhs) const {
            return val == positive_modulo(rhs);
        }

        inline bool operator==(const finite_field_template<T, P>& rhs) const {
            return val == rhs.val;
        }

        inline bool operator!=(const T& rhs) const {
            return val != positive_modulo(rhs);
        }

        inline bool operator!=(const finite_field_template<T, P>& rhs) const {
            return val != rhs.val;
        }

        inline finite_field_template<T, P> operator+(const T& rhs) const {
            return finite_field_template<T, P>(val + rhs % P);
        }

        inline finite_field_template<T, P> operator+(const finite_field_template<T, P>& rhs) const {
            return finite_field_template<T, P>(val + rhs.val);
        }

        inline finite_field_template<T, P>& operator+=(const T& rhs) {
            val = positive_modulo(val + rhs % P);
            return *this;
        }

        inline finite_field_template<T, P>& operator+=(const finite_field_template<T, P>& rhs) {
            val = (val + rhs.val) % P;
            return *this;
        }

        inline finite_field_template<T, P> operator-() const {
            return finite_field_template<T, P>(P - val);
        }

        inline finite_field_template<T, P> operator-(const T& rhs) const {
            return finite_field_template<T, P>(P + val - rhs % P);
        }

        inline finite_field_template<T, P> operator-(const finite_field_template<T, P>& rhs) const {
            return finite_field_template<T, P>(P + val - rhs.val);
        }

        inline finite_field_template<T, P>& operator-=(const T& rhs) {
            val = positive_modulo(P + val - rhs % P);
            return *this;
        }

        inline finite_field_template<T, P>& operator-=(const finite_field_template<T, P>& rhs) {
            val = positive_modulo(P + val - rhs.val);
            return *this;
        }

        inline finite_field_template<T, P> operator*(const T& rhs) const {
            return finite_field_template<T, P>(val * positive_modulo(rhs));
        }

        inline finite_field_template<T, P> operator*(const finite_field_template<T, P>& rhs) const {
            return finite_field_template<T, P>(val * rhs.val);
        }

        inline finite_field_template<T, P> operator*=(const T& rhs) {
            return *this = val * positive_modulo(rhs);
        }

        inline finite_field_template<T, P> operator*=(const finite_field_template<T, P>& rhs) {
            return *this = val * positive_modulo(rhs.val);
        }

        inline finite_field_template<T, P> operator^(int power) const {
            if (power == 0)
                return finite_field_template<T, P>(1);
            if (power == 1)
                return *this;
            
            if (power < 0) {
                return (*this ^ (P - number_utils::get_one<T>() - number_utils::get_one<T>())) ^ -power;
            }

            finite_field_template<T, P> half = *this ^ (power / 2);
            half *= half;
            if (power % 2)
                half *= *this;
            return half;
        }

        inline finite_field_template<T, P>& operator^=(int power) {
            return *this = *this ^ power;
        }

        inline finite_field_template<T, P> operator/(const T& rhs) const {
            return *this * (finite_field_template<T, P>(rhs) ^ -1);
        }

        inline finite_field_template<T, P> operator/(const finite_field_template<T, P>& rhs) const {
            return *this * (rhs ^ -1);
        }

        inline finite_field_template<T, P> operator/=(const T& rhs) {
            return *this = *this / positive_modulo(rhs);
        }

        inline finite_field_template<T, P> operator/=(const finite_field_template<T, P>& rhs) {
            return *this = *this / positive_modulo(rhs.val);
        }

        inline finite_field_template<T, P>& operator++() {
            ++val;
            val = val % P;
            return *this;
        }

        inline finite_field_template<T, P> operator++(int) {
            finite_field_template<T, P> copy = *this;
            ++*this;
            return copy;
        }

        inline finite_field_template<T, P>& operator--() {
            --val;
            val = val % P;
            return *this;
        }

        inline finite_field_template<T, P> operator--(int) {
            finite_field_template<T, P> copy = *this;
            --*this;
            return copy;
        }
    };

    template <int P>
    using int_finite_field = finite_field_template<int, P>;

    template <long long P>
    using long_finite_field = finite_field_template<long long, P>;

    template <typename T>
    class fraction {
        T numer;
        T denom;

        inline void fix() {
            do_assert(!(denom == 0), "Denominator must be non-zero");
            if (denom < number_utils::get_zero<T>()) {
                numer = -numer;
                denom = -denom;
            }
            T div = number_utils::gcd(numer < number_utils::get_zero<T>() ? -numer : numer, denom);
            numer /= div;
            denom /= div;
        }

        inline std::tuple<T&, T&> tie() {
            return std::tie(numer, denom);
        }

        inline fraction(const T& num, const T& den, int) : numer(num), denom(den) { }

    public:
        inline fraction() : numer(0), denom(1) { }
        inline fraction(const T& value) : numer(value), denom(1) { }
        inline fraction(const T& num, const T& den) : numer(num), denom(den) {
            fix();
        }

        template <typename U>
        inline fraction(const fraction<U>& rhs) : numer(rhs.numerator()), denom(rhs.denominator()) { }

        template <typename U>
        inline U value() const {
            return (U)numer / (U)denom;
        }

        inline std::pair<T, T> as_pair() const {
            return std::make_pair(numer, denom);
        }

        inline const T& numerator() const {
            return numer;
        }

        inline const T& denominator() const {
            return denom;
        }

        inline fraction<T>& operator=(const T& rhs) {
            return *this = fraction<T>(rhs, 1);
        }

        inline fraction<T>& operator=(const fraction<T>& rhs) {
            tie() = rhs.as_pair();
            return *this;
        }

        inline bool operator==(const T& rhs) const {
            return as_pair() == std::make_pair(rhs, 1);
        }

        inline bool operator==(const fraction<T>& rhs) const {
            return as_pair() == rhs.as_pair();
        }

        inline bool operator!=(const T& rhs) const {
            return as_pair() != std::make_pair(rhs, 1);
        }

        inline bool operator!=(const fraction<T>& rhs) const {
            return as_pair() != rhs.as_pair();
        }

        inline bool operator<(const T& rhs) const {
            return numer < rhs * denom;
        }

        inline bool operator<(const fraction<T>& rhs) const {
            return numer * rhs.denom < rhs.numer * denom;
        }

        inline bool operator<=(const T& rhs) const {
            return numer <= rhs * denom;
        }

        inline bool operator<=(const fraction<T>& rhs) const {
            return numer * rhs.denom <= rhs.numer * denom;
        }

        inline bool operator>(const T& rhs) const {
            return numer > rhs * denom;
        }

        inline bool operator>(const fraction<T>& rhs) const {
            return numer * rhs.denom > rhs.numer * denom;
        }

        inline bool operator>=(const T& rhs) const {
            return numer >= rhs * denom;
        }

        inline bool operator>=(const fraction<T>& rhs) const {
            return numer * rhs.denom >= rhs.numer * denom;
        }

        inline fraction<T> operator+(const T& rhs) const {
            return fraction<T>(numer + rhs * denom, denom, 0);
        }

        inline fraction<T> operator+(const fraction<T>& rhs) const {
            return fraction<T>(numer * rhs.denom + rhs.numer * denom, denom * rhs.denom);
        }

        inline fraction<T>& operator+=(const T& rhs) {
            numer += rhs * denom;
            return *this;
        }

        inline fraction<T>& operator+=(const fraction<T>& rhs) {
            numer = numer * rhs.denom + rhs.numer * denom;
            denom *= rhs.denom;
            fix();
            return *this;
        }

        inline fraction<T> operator-() const {
            return fraction<T>(-numer, denom, 0);
        }

        inline fraction<T> operator-(const T& rhs) const {
            return fraction<T>(numer - rhs * denom, denom, 0);
        }

        inline fraction<T> operator-(const fraction<T>& rhs) const {
            return fraction<T>(numer * rhs.denom - rhs.numer * denom, denom * rhs.denom);
        }

        inline fraction<T>& operator-=(const T& rhs) {
            numer -= rhs * denom;
            return *this;
        }

        inline fraction<T>& operator-=(const fraction<T>& rhs) {
            numer = numer * rhs.denom - rhs.numer * denom;
            denom *= rhs.denom;
            fix();
            return *this;
        }

        inline fraction<T> operator*(const T& rhs) const {
            return fraction<T>(numer * rhs, denom);
        }

        inline fraction<T> operator*(const fraction<T>& rhs) const {
            return fraction<T>(numer * rhs.numer, denom * rhs.denom);
        }

        inline fraction<T> operator*=(const T& rhs) {
            numer *= rhs;
            fix();
            return *this;
        }

        inline fraction<T> operator*=(const fraction<T>& rhs) {
            numer *= rhs.numer;
            denom *= rhs.denom;
            fix();
            return *this;
        }

        inline fraction<T> operator/(const T& rhs) const {
            return fraction<T>(numer, denom * rhs);
        }

        inline fraction<T> operator/(const fraction<T>& rhs) const {
            return fraction<T>(numer * rhs.denom, denom * rhs.numer);
        }

        inline fraction<T> operator/=(const T& rhs) {
            denom *= rhs;
            fix();
            return *this;
        }

        inline fraction<T> operator/=(const fraction<T>& rhs) {
            numer *= rhs.denom;
            denom *= rhs.numer;
            fix();
            return *this;
        }

        inline fraction<T> operator^(int power) const {
            if (power == 0)
                return fraction<T>(1, 1);
            if (power == 1)
                return *this;
            
            if (power < 0) {
                return fraction<T>(denom, numer, 0) ^ -power;
            }

            fraction<T> half = *this ^ (power / 2);
            half *= half;
            if (power % 2)
                half *= *this;
            return half;
        }

        inline fraction<T>& operator^=(int power) {
            return *this = *this ^ power;
        }

        inline fraction<T>& operator++() {
            numer += denom;
            return *this;
        }

        inline fraction<T> operator++(int) {
            fraction<T> copy = *this;
            ++*this;
            return copy;
        }

        inline fraction<T>& operator--() {
            numer -= denom;
            return *this;
        }

        inline fraction<T> operator--(int) {
            fraction<T> copy = *this;
            --*this;
            return copy;
        }
    };

};

namespace number_utils {

    template <typename U>
    struct standard_numbers<matrices::finite_field<U>> {
        static inline matrices::finite_field<U> zero() = delete;

        static inline matrices::finite_field<U> one() = delete;

        static inline matrices::finite_field<U> minus_one() = delete;

        static inline matrices::finite_field<U> zero(const matrices::finite_field<U>& sample) {
            return matrices::finite_field<U>(sample.size(), get_zero<U>());
        }

        static inline matrices::finite_field<U> one(const matrices::finite_field<U>& sample) {
            return matrices::finite_field<U>(sample.size(), get_one<U>());
        }

        static inline matrices::finite_field<U> minus_one(const matrices::finite_field<U>& sample) {
            return matrices::finite_field<U>(sample.size(), get_minus_one<U>());
        }
    };

    template <typename U, U P>
    struct standard_numbers<matrices::finite_field_template<U, P>> {
        static inline matrices::finite_field_template<U, P> zero() {
            return matrices::finite_field_template<U, P>(get_zero<U>());
        }

        static inline matrices::finite_field_template<U, P> one() {
            return matrices::finite_field_template<U, P>(get_one<U>());
        }

        static inline matrices::finite_field_template<U, P> minus_one() {
            return matrices::finite_field_template<U, P>(P - get_one<U>());
        }

        static inline matrices::finite_field_template<U, P> zero(const matrices::finite_field_template<U, P>& sample) {
            return matrices::finite_field_template<U, P>(get_zero<U>());
        }

        static inline matrices::finite_field_template<U, P> one(const matrices::finite_field_template<U, P>& sample) {
            return matrices::finite_field_template<U, P>(get_one<U>());
        }

        static inline matrices::finite_field_template<U, P> minus_one(const matrices::finite_field_template<U, P>& sample) {
            return matrices::finite_field_template<U, P>(P - get_one<U>());
        }
    };


    template <typename U>
    struct standard_numbers<matrices::fraction<U>> {
        static inline matrices::fraction<U> zero() {
            return matrices::fraction<U>(get_zero<U>(), get_one<U>());
        }

        static inline matrices::fraction<U> one() {
            return matrices::fraction<U>(get_one<U>(), get_one<U>());
        }

        static inline matrices::fraction<U> minus_one() {
            return matrices::fraction<U>(get_minus_one<U>(), get_one<U>());
        }

        static inline matrices::fraction<U> zero(const matrices::fraction<U>& sample) {
            return matrices::fraction<U>(get_zero<U>(), get_one<U>());
        }

        static inline matrices::fraction<U> one(const matrices::fraction<U>& sample) {
            return matrices::fraction<U>(get_one<U>(), get_one<U>());
        }

        static inline matrices::fraction<U> minus_one(const matrices::fraction<U>& sample) {
            return matrices::fraction<U>(get_minus_one<U>(), get_one<U>());
        }
    };

};
