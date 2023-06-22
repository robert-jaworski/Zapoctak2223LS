#pragma once

#include <vector>

#include "numbers.hpp"
#include "bigint.hpp"

namespace number_utils {

    constexpr unsigned long long BIGINT10_BASE = 1'000'000'000;

    class bigint10 {
    protected:
        using ull = unsigned long long;

        std::vector<ull>* digits = new std::vector<ull>();
        bool negative = false;

        inline void fix() {
            while (digits->size() > 1 && digits->back() == 0) {
                digits->pop_back();
            }
            if (digits->size() == 0) digits->push_back(0);
            if (is_zero()) negative = false;
        }

    public:
        inline ~bigint10() {
            delete digits;
        }

        inline bigint10(const std::vector<ull>& d, bool neg = false) : negative(neg) {
            *digits = d;
            fix();
        }

        inline bigint10(digits_view<ull> d, bool neg = false) : negative(neg) {
            digits->resize(d.size());
            for (int i = 0; i < d.size(); i++) {
                digits->at(i) = d[i];
            }
            fix();
        }

        inline bigint10(ull x = 0) {
            digits->push_back(x % BIGINT10_BASE);
            digits->push_back(x / BIGINT10_BASE);
            fix();
        }

        inline bigint10(size_t x) {
            digits->push_back(x % BIGINT10_BASE);
            digits->push_back(x / BIGINT10_BASE);
            fix();
        }

        inline bigint10(long long x) {
            auto u = (ull)abs(x);
            negative = x < 0;
            digits->push_back(u % BIGINT10_BASE);
            digits->push_back(u / BIGINT10_BASE);
            fix();
        }

        inline bigint10(int x) {
            auto u = (ull)abs(x);
            negative = x < 0;
            digits->push_back(u % BIGINT10_BASE);
            digits->push_back(u / BIGINT10_BASE);
            fix();
        }

        inline bigint10(const bigint10& x) {
            *digits = *x.digits;
            negative = x.negative;
        }

        inline bigint10(bigint10&& x) {
            std::swap(digits, x.digits);
            negative = x.negative;
        }

    #ifdef DEBUG
        ull f() const;
    #else
        ;
    #endif

        inline void swap_with(bigint10& rhs) {
            std::swap(digits, rhs.digits);
            std::swap(negative, rhs.negative);
        }

        inline size_t digit_count() const {
            return digits->size();
        }

        inline bigint10& operator=(const bigint10& rhs) {
            *digits = *rhs.digits;
            negative = rhs.negative;
            return *this;
        }

        inline bigint10& operator=(bigint10&& rhs) {
            std::swap(digits, rhs.digits);
            negative = rhs.negative;
            return *this;
        }

        inline bool is_zero() const {
            return digits->size() == 1 && digits->at(0) == 0;
        }

        inline bool is_one(bool neg = false) const {
            return digits->size() == 1 && digits->at(0) == 1 && negative == neg;
        }

        inline bool is_two(bool neg = false) const {
            return digits->size() == 1 && digits->at(0) == 2 && negative == neg;
        }

        inline bool is_ten(bool neg = false) const {
            return digits->size() == 1 && digits->at(0) == 10 && negative == neg;
        }

        inline bool is_positive() const {
            return !negative;
        }

        inline bool is_negative() const {
            return negative;
        }

    protected:
        static int compare(digits_view<ull> a, digits_view<ull> b, bool noStartZeros = true);
        static int compare_mult(digits_view<ull> a, digits_view<ull> b, size_t offset, ull mult, bool noStartZeros = true);

        inline int compare_with(const bigint10& rhs) const {
            return compare(digits, rhs.digits);
        }

    public:
        inline bool operator==(const bigint10& rhs) const {
            return negative == rhs.negative && *digits == *rhs.digits;
        }

        inline bool operator!=(const bigint10& rhs) const {
            return negative != rhs.negative || *digits != *rhs.digits;
        }

        inline bool operator<(const bigint10& rhs) const {
            if (negative && !rhs.negative) return true;
            if (!negative && rhs.negative) return false;
            int comp = compare_with(rhs);
            if (negative) return comp == 1;
            return comp == -1;
        }

        inline bool operator<=(const bigint10& rhs) const {
            if (negative && !rhs.negative) return true;
            if (!negative && rhs.negative) return false;
            int comp = compare_with(rhs);
            if (negative) return comp != -1;
            return comp != 1;
        }

        inline bool operator>(const bigint10& rhs) const {
            if (negative && !rhs.negative) return false;
            if (!negative && rhs.negative) return true;
            int comp = compare_with(rhs);
            if (negative) return comp == -1;
            return comp == 1;
        }

        inline bool operator>=(const bigint10& rhs) const {
            if (negative && !rhs.negative) return false;
            if (!negative && rhs.negative) return true;
            int comp = compare_with(rhs);
            if (negative) return comp != 1;
            return comp != -1;
        }

        inline bigint10& operator++() {
            return operator+=(1ULL);
        }

        inline bigint10 operator++(int) {
            bigint10 tmp = *this;
            operator+=(1ULL);
            return tmp;
        }

        inline bigint10& operator--() {
            return operator-=(1ULL);
        }

        inline bigint10 operator--(int) {
            bigint10 tmp = *this;
            operator-=(1ULL);
            return tmp;
        }

        inline bigint10 operator-() const {
            return { *digits, !negative };
        }

    protected:
        static void add_digits(std::vector<ull>* res, digits_view<ull> a, digits_view<ull> b);
        static void add_digits_inplace(std::vector<ull>* a, digits_view<ull> b);

        static void sub_digits_no_neg(std::vector<ull>* res, digits_view<ull> a, digits_view<ull> b);
        static void sub_digits_no_neg_inplace(std::vector<ull>* a, digits_view<ull> b);

        static bool do_add(std::vector<ull>* res, digits_view<ull> a, digits_view<ull> b, bool aNeg, bool bNeg);
        static std::pair<bool, std::vector<ull>*> do_add_inplace(std::vector<ull>* a, digits_view<ull> b, bool aNeg, bool bNeg);

        static void add_digits_inplace_mult(std::vector<ull>* a, digits_view<ull> b, size_t offset, ull mult);
        static bool do_mult(std::vector<ull>* res, digits_view<ull> a, digits_view<ull> b, bool aNeg, bool bNeg);
        static void mult_digits(std::vector<ull>* res, digits_view<ull> a, digits_view<ull> b);

        static bool divide(std::vector<ull>* q, std::vector<ull>* r, digits_view<ull> a, digits_view<ull> b, bool aNeg, bool bNeg);

    public:
        inline bigint10 operator+(const bigint10& rhs) const {
            bigint10 res;
            res.negative = do_add(res.digits, digits, rhs.digits, negative, rhs.negative);
            res.fix();
            return res;
        }

        inline bigint10& operator+=(const bigint10& rhs) {
            std::vector<ull>* res;
            std::tie(negative, res) = do_add_inplace(digits, rhs.digits, negative, rhs.negative);
            if (res != digits) {
                delete digits;
                digits = res;
            }
            return *this;
        }

        inline bigint10 operator-(const bigint10& rhs) const {
            bigint10 res;
            res.negative = do_add(res.digits, digits, rhs.digits, negative, !rhs.negative);
            res.fix();
            return res;
        }

        inline bigint10& operator-=(const bigint10& rhs) {
            std::vector<ull>* res;
            std::tie(negative, res) = do_add_inplace(digits, rhs.digits, negative, !rhs.negative);
            if (res != digits) {
                delete digits;
                digits = res;
            }
            return *this;
        }

        inline bigint10 operator*(const bigint10& rhs) const {
            bigint10 res;
            if (is_zero() || rhs.is_zero()) {
                return 0;
            }
            if (is_one()) {
                return rhs;
            }
            if (is_one(true)) {
                return -rhs;
            }
            if (rhs.is_one()) {
                return *this;
            }
            if (rhs.is_one(true)) {
                return -*this;
            }
            if (is_two()) {
                return rhs + rhs;
            }
            if (is_two(true)) {
                return -(rhs + rhs);
            }
            if (rhs.is_two()) {
                return *this + *this;
            }
            if (rhs.is_two(true)) {
                return -(*this + *this);
            }
            res.negative = do_mult(res.digits, digits, rhs.digits, negative, rhs.negative);
            res.fix();
            return res;
        }

        inline bigint10& operator*=(const bigint10& rhs) {
            std::vector<ull>* res = new std::vector<ull>();
            negative = do_mult(res, digits, rhs.digits, negative, rhs.negative);
            delete digits;
            digits = res;
            return *this;
        }

        std::pair<bigint10, bigint10> integer_divide(const bigint10& rhs) const;

        inline bigint10 operator/(const bigint10& rhs) const {
            return integer_divide(rhs).first;
        }

        inline bigint10& operator/=(const bigint10& rhs) {
            *this = integer_divide(rhs).first;
            return *this;
        }

        inline bigint10 operator%(const bigint10& rhs) const {
            return integer_divide(rhs).second;
        }

        inline bigint10& operator%=(const bigint10& rhs) {
            *this = integer_divide(rhs).second;
            return *this;
        }

        inline bigint10 operator^(const bigint10& rhs) const {
            if (rhs.is_zero()) {
                if (is_zero()) throw std::logic_error("0^0 is indeterminate");
                return 1LL;
            }
            if (rhs.is_one()) return *this;
            if (rhs.is_negative()) {
                throw std::logic_error("BigInt10 does not support fractions");
            }
            bigint10 tmp = operator^(rhs / 2);
            tmp *= tmp;
            if (rhs.digits->at(0) % 2 == 1) tmp *= *this;
            return tmp;
        }

        inline const bigint10& operator^=(const bigint10& rhs) {
            bigint10 res = operator^(rhs);
            swap_with(res);
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const bigint10& obj);

        friend std::istream& operator>>(std::istream& is, bigint10& obj);

        std::string create_base_string(int base, bool uppercase) const;

        static bigint10 read_from_string(std::string str, int base);

        inline ull lowest_digit() const {
            return digits->at(0);
        }
    };

    template<>
    constexpr inline bool will_add_overflow<bigint10>(const bigint10& a, const bigint10& b) {
        return false;
    }

    template<>
    constexpr inline bool can_mul_overflow<bigint10>(const bigint10& a, const bigint10& b) {
        return false;
    }

    inline bigint10 operator""_BI10(unsigned long long x) {
        return bigint10(x);
    }

    std::ostream& operator<<(std::ostream& os, const bigint10& obj);

    std::istream& operator>>(std::istream& is, bigint10& obj);

}

template<>
inline void std::swap(number_utils::bigint10& a, number_utils::bigint10& b) {
    a.swap_with(b);
}
