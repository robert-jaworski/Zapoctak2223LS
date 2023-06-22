#pragma once

#include <vector>
#include <tuple>

#include "numbers.hpp"
#include "bigint10.hpp"

namespace number_utils {

    constexpr int BIGINT_BASE_BITS = 32;
    constexpr unsigned long long BIGINT_BASE = 1ULL << BIGINT_BASE_BITS;

    class bigint10;
    extern std::vector<bigint10> BIGINT_BASE_POWERS_BIGINT10;
    extern bigint10 BIGINT_BASE_BIGINT10;

    class bigint {
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
        inline ~bigint() {
            delete digits;
        }

        inline bigint(const std::vector<ull>& d, bool neg = false) : negative(neg) {
            *digits = d;
            fix();
        }

        inline bigint(digits_view<ull> d, bool neg = false) : negative(neg) {
            digits->resize(d.size());
            for (int i = 0; i < d.size(); i++) {
                digits->at(i) = d[i];
            }
            fix();
        }

        inline bigint(ull x = 0) {
            digits->push_back(x % BIGINT_BASE);
            digits->push_back(x / BIGINT_BASE);
            fix();
        }

        inline bigint(size_t x) {
            digits->push_back(x % BIGINT_BASE);
            digits->push_back(x / BIGINT_BASE);
            fix();
        }

        inline bigint(long long x) {
            auto u = (ull)abs(x);
            negative = x < 0;
            digits->push_back(u % BIGINT_BASE);
            digits->push_back(u / BIGINT_BASE);
            fix();
        }

        inline bigint(int x) {
            auto u = (ull)abs(x);
            negative = x < 0;
            digits->push_back(u % BIGINT_BASE);
            digits->push_back(u / BIGINT_BASE);
            fix();
        }

        inline bigint(const bigint& x) {
            *digits = *x.digits;
            negative = x.negative;
        }

        inline bigint(bigint&& x) {
            std::swap(digits, x.digits);
            negative = x.negative;
        }

    #ifdef DEBUG
        ull f() const;
    #else
        ;
    #endif

        inline void swap_with(bigint& rhs) {
            std::swap(digits, rhs.digits);
            std::swap(negative, rhs.negative);
        }

        inline size_t digit_count() const {
            return digits->size();
        }

        inline bigint& operator=(const bigint& rhs) {
            *digits = *rhs.digits;
            negative = rhs.negative;
            return *this;
        }

        inline bigint& operator=(bigint&& rhs) {
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

        inline bool is_positive() const {
            return !negative;
        }

        inline bool is_negative() const {
            return negative;
        }

    protected:
        static int compare(digits_view<ull> a, digits_view<ull> b, bool noStartZeros = true);
        static int compare_mult(digits_view<ull> a, digits_view<ull> b, size_t offset, ull mult, bool noStartZeros = true);

        inline int compare_with(const bigint& rhs) const {
            return compare(digits, rhs.digits);
        }

    public:
        inline bool operator==(const bigint& rhs) const {
            return negative == rhs.negative && *digits == *rhs.digits;
        }

        inline bool operator!=(const bigint& rhs) const {
            return negative != rhs.negative || *digits != *rhs.digits;
        }

        inline bool operator<(const bigint& rhs) const {
            if (negative && !rhs.negative) return true;
            if (!negative && rhs.negative) return false;
            int comp = compare_with(rhs);
            if (negative) return comp == 1;
            return comp == -1;
        }

        inline bool operator<=(const bigint& rhs) const {
            if (negative && !rhs.negative) return true;
            if (!negative && rhs.negative) return false;
            int comp = compare_with(rhs);
            if (negative) return comp != -1;
            return comp != 1;
        }

        inline bool operator>(const bigint& rhs) const {
            if (negative && !rhs.negative) return false;
            if (!negative && rhs.negative) return true;
            int comp = compare_with(rhs);
            if (negative) return comp == -1;
            return comp == 1;
        }

        inline bool operator>=(const bigint& rhs) const {
            if (negative && !rhs.negative) return false;
            if (!negative && rhs.negative) return true;
            int comp = compare_with(rhs);
            if (negative) return comp != 1;
            return comp != -1;
        }

        inline bigint& operator++() {
            return operator+=(1ULL);
        }

        inline bigint operator++(int) {
            bigint tmp = *this;
            operator+=(1ULL);
            return tmp;
        }

        inline bigint& operator--() {
            return operator-=(1ULL);
        }

        inline bigint operator--(int) {
            bigint tmp = *this;
            operator-=(1ULL);
            return tmp;
        }

        inline bigint operator-() const {
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
        inline bigint operator+(const bigint& rhs) const {
            bigint res;
            res.negative = do_add(res.digits, digits, rhs.digits, negative, rhs.negative);
            res.fix();
            return res;
        }

        inline bigint& operator+=(const bigint& rhs) {
            std::vector<ull>* res;
            std::tie(negative, res) = do_add_inplace(digits, rhs.digits, negative, rhs.negative);
            if (res != digits) {
                delete digits;
                digits = res;
            }
            return *this;
        }

        inline bigint operator-(const bigint& rhs) const {
            bigint res;
            res.negative = do_add(res.digits, digits, rhs.digits, negative, !rhs.negative);
            res.fix();
            return res;
        }

        inline bigint& operator-=(const bigint& rhs) {
            std::vector<ull>* res;
            std::tie(negative, res) = do_add_inplace(digits, rhs.digits, negative, !rhs.negative);
            if (res != digits) {
                delete digits;
                digits = res;
            }
            return *this;
        }

        inline bigint operator*(const bigint& rhs) const {
            bigint res;
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
            if (rhs.digits->size() == 1) {
                if (is_power_of_2(rhs.digits->at(0))) {
                    res = operator<<(get_log(rhs.digits->at(0), 2ULL));
                    return res;
                }
            }
            res.negative = do_mult(res.digits, digits, rhs.digits, negative, rhs.negative);
            res.fix();
            return res;
        }

        inline bigint& operator*=(const bigint& rhs) {
            std::vector<ull>* res = new std::vector<ull>();
            negative = do_mult(res, digits, rhs.digits, negative, rhs.negative);
            delete digits;
            digits = res;
            return *this;
        }

        inline bigint operator>>(size_t x) const {
            bigint res;
            res.negative = negative;
            res.digits->clear();
            int off = x / BIGINT_BASE_BITS;
            int sh = x % BIGINT_BASE_BITS;
            for (int i = 0; i + off < digits->size(); i++) {
                res.digits->push_back(digits->at(i + off) >> sh);
                if (i + off + 1 < digits->size()) {
                    res.digits->back() += digits->at(i + off + 1) << (32 - sh);
                    res.digits->back() %= BIGINT_BASE;
                }
            }
            res.fix();
            return res;
        }

        inline bigint& operator>>=(size_t x) {
            *this = operator>>(x);
            return *this;
        }

        inline bigint operator<<(ull x) const {
            bigint res;
            res.negative = negative;
            res.digits->clear();
            ull off = x / BIGINT_BASE_BITS;
            ull sh = x % BIGINT_BASE_BITS;
            res.digits->resize(off);
            for (int i = 0; i - off < (int)digits->size(); i++) {
                res.digits->push_back(digits->at(i - off) << sh);
                if (i - (long long)off - 1 >= 0) {
                    res.digits->back() += digits->at(i - off - 1) >> (32 - sh);
                }
                res.digits->back() %= BIGINT_BASE;
            }
            res.fix();
            return res;
        }

        inline bigint& operator<<=(size_t x) {
            *this = operator<<(x);
            return *this;
        }

        std::pair<bigint, bigint> integer_divide(const bigint& rhs) const;

        inline bigint operator/(const bigint& rhs) const {
            return integer_divide(rhs).first;
        }

        inline bigint& operator/=(const bigint& rhs) {
            *this = integer_divide(rhs).first;
            return *this;
        }

        inline bigint operator%(const bigint& rhs) const {
            return integer_divide(rhs).second;
        }

        inline bigint& operator%=(const bigint& rhs) {
            *this = integer_divide(rhs).second;
            return *this;
        }

        inline bigint operator^(const bigint& rhs) const {
            if (rhs.is_zero()) {
                return 1LL;
            }
            if (rhs.is_one()) return *this;
            if (rhs.is_negative()) {
                throw std::logic_error("bigint does not support fractions");
            }
            bigint tmp = operator^(rhs >> 1);
            tmp *= tmp;
            if (rhs.digits->at(0) % 2 == 1) tmp *= *this;
            return tmp;
        }

        inline const bigint& operator^=(const bigint& rhs) {
            bigint res = operator^(rhs);
            swap_with(res);
            return *this;
        }

    protected:
        static bigint10 digits_to_bigint10(digits_view<ull> d);

    public:
        bigint10 to_bigint10() const;

        friend std::ostream& operator<<(std::ostream& os, const bigint& obj);

        friend std::istream& operator>>(std::istream& is, bigint& obj);

        std::string create_base_string(int base, bool uppercase) const;

        static bigint read_from_string(std::string str, int base);

        inline ull lowest_digit() const {
            return digits->at(0);
        }
    };

    template<>
    constexpr inline bool will_add_overflow<bigint>(const bigint& a, const bigint& b) {
        return false;
    }

    template<>
    constexpr inline bool can_mul_overflow<bigint>(const bigint& a, const bigint& b) {
        return false;
    }

    inline bigint operator""_BI(unsigned long long x) {
        return bigint(x);
    }

    std::ostream& operator<<(std::ostream& os, const bigint& obj);

    std::istream& operator>>(std::istream& is, bigint& obj);

}

template<>
inline void std::swap(number_utils::bigint& a, number_utils::bigint& b) {
    a.swap_with(b);
}
