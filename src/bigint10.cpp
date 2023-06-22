#include <iostream>
#include <iomanip>
#include <sstream>
#include "assert.h"
#include "bigint10.hpp"

using namespace std;
typedef unsigned long long ull;

namespace number_utils {

    #ifdef DEBUG
    ull bigint10::f() const {
        return digits->at(0);
    }
    #endif

    int bigint10::compare(digits_view<ull> a, digits_view<ull> b, bool noStartZeros) {
        if (a.size() == 0 && b.size() == 0) return 0;
        if (noStartZeros) {
            if (a.size() > b.size()) return 1;
            if (a.size() < b.size()) return -1;
        }
        else {
            if (a.size() > b.size()) {
                for (int i = a.size() - 1; i >= b.size(); i--) {
                    if (a.at(i)) return 1;
                }
            }
            if (a.size() < b.size()) {
                for (int i = b.size() - 1; i >= a.size(); i--) {
                    if (b.at(i)) return -1;
                }
            }
        }

        for (int i = min(a.size(), b.size()) - 1; i >= 0; i--) {
            if (a.at(i) > b.at(i)) return 1;
            if (a.at(i) < b.at(i)) return -1;
        }
        return 0;
    }

    int bigint10::compare_mult(digits_view<ull> a, digits_view<ull> b, size_t offset, ull mult, bool noStartZeros) {
        vector<ull> tmp;
        add_digits_inplace_mult(&tmp, b, offset, mult);
        return compare(a, tmp, noStartZeros);
    }

    void bigint10::add_digits(vector<ull>* res, digits_view<ull> a, digits_view<ull> b) {
        res->clear();
        res->reserve(max(a.size(), b.size()) + 1);
        ull carry = 0;
        for (int i = 0; i < a.size() || i < b.size(); i++) {
            if (i >= a.size()) {
                res->push_back(b.at(i) + carry);
            }
            else if (i >= b.size()) {
                res->push_back(a.at(i) + carry);
            }
            else {
                res->push_back(a.at(i) + b.at(i) + carry);
            }
            carry = res->back() / BIGINT10_BASE;
            res->back() %= BIGINT10_BASE;
        }
        if (carry) res->push_back(carry);
    }

    void bigint10::add_digits_inplace(vector<ull>* a, digits_view<ull> b) {
        a->reserve(max(a->size(), b.size()) + 1);
        ull carry = 0;
        for (int i = 0; i < a->size() || i < b.size(); i++) {
            if (i >= a->size()) {
                a->push_back(b.at(i) + carry);
            }
            else if (i >= b.size()) {
                if (!carry) break;
                a->at(i) += carry;
            }
            else {
                a->at(i) += b.at(i) + carry;
            }
            carry = a->at(i) / BIGINT10_BASE;
            a->at(i) %= BIGINT10_BASE;
        }
        if (carry) a->push_back(carry);
    }

    void bigint10::sub_digits_no_neg(vector<ull>* res, digits_view<ull> a, digits_view<ull> b) {
        assert(a.size() >= b.size());
        res->clear();
        res->reserve(a.size());
        ull carry = 0;
        for (int i = 0; i < a.size(); i++) {
            if (i >= b.size()) {
                auto aa = a.at(i);
                auto bb = carry;
                carry = 0;
                if (bb > aa) {
                    carry = 1;
                    aa += BIGINT10_BASE;
                }
                res->push_back(aa - bb);
            }
            else {
                auto aa = a.at(i);
                auto bb = b.at(i) + carry;
                carry = 0;
                if (bb > aa) {
                    carry = 1;
                    aa += BIGINT10_BASE;
                }
                res->push_back(aa - bb);
            }
        }
    }

    void bigint10::sub_digits_no_neg_inplace(vector<ull>* a, digits_view<ull> b) {
        assert(a->size() >= b.size());
        ull carry = 0;
        for (int i = 0; i < a->size(); i++) {
            if (i >= b.size()) {
                if (!carry) break;
                auto aa = a->at(i);
                auto bb = carry;
                carry = 0;
                if (bb > aa) {
                    carry = 1;
                    aa += BIGINT10_BASE;
                }
                a->at(i) = aa - bb;
            }
            else {
                auto aa = a->at(i);
                auto bb = b.at(i) + carry;
                carry = 0;
                if (bb > aa) {
                    carry = 1;
                    aa += BIGINT10_BASE;
                }
                a->at(i) = aa - bb;
            }
        }
    }

    bool bigint10::do_add(vector<ull>* res, digits_view<ull> a, digits_view<ull> b, bool aNeg, bool bNeg) {
        if (aNeg == bNeg) {
            add_digits(res, a, b);
            return aNeg;
        }
        else {
            int comp = compare(a, b);
            if (comp == 0) {
                res->clear();
                return false;
            }
            if (aNeg) {
                if (comp == 1) {
                    sub_digits_no_neg(res, a, b);
                    return true;
                }
                else {
                    sub_digits_no_neg(res, b, a);
                }
            }
            else {
                if (comp == 1) {
                    sub_digits_no_neg(res, a, b);
                }
                else {
                    sub_digits_no_neg(res, b, a);
                    return true;
                }
            }
        }
        return false;
    }

    pair<bool, vector<ull>*> bigint10::do_add_inplace(std::vector<ull>* a, digits_view<ull> b, bool aNeg, bool bNeg) {
        if (aNeg == bNeg) {
            add_digits_inplace(a, b);
            return make_pair(aNeg, a);
        }
        else {
            int comp = compare(a, b);
            if (comp == 0) {
                a->clear();
                a->push_back(0);
                return make_pair(false, a);
            }
            if (aNeg) {
                if (comp == 1) {
                    sub_digits_no_neg_inplace(a, b);
                    return make_pair(true, a);
                }
                else {
                    vector<ull>* res = new vector<ull>();
                    sub_digits_no_neg(res, b, a);
                    return make_pair(false, res);
                }
            }
            else {
                if (comp == 1) {
                    sub_digits_no_neg_inplace(a, b);
                }
                else {
                    vector<ull>* res = new vector<ull>();
                    sub_digits_no_neg(res, b, a);
                    return make_pair(true, res);
                }
            }
        }
        return make_pair(false, a);
    }

    void bigint10::add_digits_inplace_mult(std::vector<ull>* a, digits_view<ull> b, size_t offset, ull mult) {
        if (a->size() < offset) {
            a->resize(offset);
        }
        a->reserve(max(a->size(), b.size() + offset) + 1);
        ull carry = 0;
        for (int i = 0; i + offset < a->size() || i < b.size(); i++) {
            if (i + offset >= a->size()) {
                a->push_back(b.at(i) * mult + carry);
            }
            else if (i >= b.size()) {
                if (!carry) break;
                a->at(i + offset) += carry;
            }
            else {
                a->at(i + offset) += b.at(i) * mult + carry;
            }
            carry = a->at(i + offset) / BIGINT10_BASE;
            a->at(i + offset) %= BIGINT10_BASE;
        }
        if (carry) a->push_back(carry);
    }

    bool bigint10::do_mult(std::vector<ull>* res, digits_view<ull> a, digits_view<ull> b, bool aNeg, bool bNeg) {
        mult_digits(res, a, b);
        return aNeg != bNeg;
    }

    void bigint10::mult_digits(std::vector<ull>* res, digits_view<ull> a, digits_view<ull> b) {
        if (a.size() == 1) {
            add_digits_inplace_mult(res, b, 0, a[0]);
            return;
        }
        if (b.size() == 1) {
            add_digits_inplace_mult(res, a, 0, b[0]);
            return;
        }
        
        int size1 = max(a.size(), b.size()) / 2;
        //int size2 = max(a.size(), b.size()) - size1;
        if (a.size() < size1) {
            digits_view<ull> bf = b.view(0, size1);
            digits_view<ull> bl = b.view(size1, b.size() - size1);

            vector<ull> abf;
            vector<ull> abl;
            mult_digits(&abf, a, bf);
            mult_digits(&abl, a, bl);
            
            add_digits_inplace_mult(res, abf, 0, 1);
            add_digits_inplace_mult(res, abl, size1, 1);
            return;
        }
        if (b.size() < size1) {
            digits_view<ull> af = a.view(0, size1);
            digits_view<ull> al = a.view(size1, a.size() - size1);

            vector<ull> baf;
            vector<ull> bal;
            mult_digits(&baf, b, af);
            mult_digits(&bal, b, al);
            
            add_digits_inplace_mult(res, baf, 0, 1);
            add_digits_inplace_mult(res, bal, size1, 1);
            return;
        }

        digits_view<ull> af = a.view(0, size1);
        digits_view<ull> al = a.view(size1, a.size() - size1);
        digits_view<ull> bf = b.view(0, size1);
        digits_view<ull> bl = b.view(size1, b.size() - size1);

        vector<ull> afbf;
        vector<ull> albl;
        mult_digits(&afbf, af, bf);
        mult_digits(&albl, al, bl);

        vector<ull> as;
        vector<ull> bs;
        add_digits(&as, af, al);
        add_digits(&bs, bf, bl);
        vector<ull> asbs;
        mult_digits(&asbs, as, bs);
        sub_digits_no_neg_inplace(&asbs, afbf);
        sub_digits_no_neg_inplace(&asbs, albl);

        add_digits_inplace_mult(res, afbf, 0, 1);
        add_digits_inplace_mult(res, asbs, size1, 1);
        add_digits_inplace_mult(res, albl, size1 * 2, 1);
    }

    std::pair<bigint10, bigint10> bigint10::integer_divide(const bigint10& rhs) const {
        if (rhs.is_zero()) {
            throw std::logic_error("Division by zero");
        }
        if (is_zero()) {
            return std::make_pair(*this, 0ULL);
        }
        if (rhs.is_one()) {
            return std::make_pair(*this, 0ULL);
        }
        if (rhs.is_one(true)) {
            bigint10 res = *this;
            res.negative = !negative;
            return std::make_pair(res, 0ULL);
        }
        if (rhs.is_two()) {
            bigint10 q = digits->at(0) / 2, r = digits->at(0) % 2;
            q.digits->resize(digits->size());
            for (int i = 1; i < digits->size(); i++) {
                q.digits->at(i) = digits->at(i) / 2;
                q.digits->at(i - 1) += (digits->at(i) % 2) * BIGINT10_BASE / 2;
            }
            q.fix();
            return make_pair(q, r);
        }
        if (rhs.is_two(true)) {
            auto res = integer_divide(2ULL);
            res.first.negative = !res.first.negative;
            res.second.negative = !res.second.negative;
            return res;
        }
        if (rhs.is_ten()) {
            bigint10 q = digits->at(0) / 10, r = digits->at(0) % 10;
            q.digits->resize(digits->size());
            for (int i = 1; i < digits->size(); i++) {
                q.digits->at(i) = digits->at(i) / 10;
                q.digits->at(i - 1) += (digits->at(i) % 10) * BIGINT10_BASE / 10;
            }
            q.fix();
            return make_pair(q, r);
        }
        if (rhs.is_ten(true)) {
            auto res = integer_divide(10ULL);
            res.first.negative = !res.first.negative;
            res.second.negative = !res.second.negative;
            return res;
        }

        bigint10 q, r;
        q.negative = r.negative = divide(q.digits, r.digits, digits, rhs.digits, negative, rhs.negative);
        r.fix();
        q.fix();
        return std::make_pair(q, r);
    }

    bool bigint10::divide(std::vector<ull>* q, std::vector<ull>* r, digits_view<ull> a, digits_view<ull> b, bool aNeg, bool bNeg) {
        int off = a.size() - b.size();

        if (off < 0) {
            q->resize(1, 0);
            r->resize(a.size());
            for (int i = 0; i < a.size(); i++) {
                r->at(i) = a[i];
            }
            return aNeg != bNeg;
        }

        q->resize(off + 1);
        r->resize(a.size());
        for (int i = 0; i < a.size(); i++) {
            r->at(i) = a[i];
        }

        vector<ull> sub;
        while (off >= 0) {
            ull left = 0, right = 0;
            if (b.size() == 1) {
                ull A = r->at(b.size() - 1 + off);
                if (b.size() + off < r->size()) A += r->at(b.size() + off) * BIGINT10_BASE;
                left = right = A / b.back();
            }
            else {
                ull A, B;
                if (b.size() + off >= r->size() || r->at(b.size() + off) == 0) {
                    A = r->at(b.size() - 1 + off) * BIGINT10_BASE + r->at(b.size() - 2 + off);
                    B = b.at(b.size() - 1) * BIGINT10_BASE + b.at(b.size() - 2);
                }
                else {
                    A = r->at(b.size() + off) * BIGINT10_BASE + r->at(b.size() - 1 + off);
                    B = b.back();
                }
                left = A / (B + 1);
                right = A / B;
            }
            while (left < right) {
                ull middle = (left + right + 1) / 2;
                int comp = compare_mult(r, b, off, middle, false);
                if (comp == 0) {
                    left = right = middle;
                }
                else if (comp == -1) {
                    right = middle - 1;
                }
                else if (comp == 1) {
                    left = middle;
                }
            }
            sub.clear();
            add_digits_inplace_mult(&sub, b, off, left);
            sub_digits_no_neg_inplace(r, sub);
            q->at(off) = left;
            off--;
        }
        return aNeg != bNeg;
    }

    #define DIGIT_COUNT 36
    #define ERR_MSG2(X) "Only bases from 2 to " #X " are supported"
    #define ERR_MSG ERR_MSG2(DIGIT_COUNT)

    std::string bigint10::create_base_string(int base, bool uppercase) const {
        constexpr char DIGITS[2 * DIGIT_COUNT + 1] = "0123456789abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        if (base > DIGIT_COUNT || base < 2) {
            throw std::logic_error(ERR_MSG);
        }
        if (is_zero()) return "0";
        std::string res;
        bigint10 curr = *this, q, r, div = base;
        int off = uppercase ? 36 : 0;
        while (!curr.is_zero()) {
            std::tie(q, r) = curr.integer_divide(div);
            res = DIGITS[r.digits->at(0) + off] + res;
            curr = q;
        }
        if (is_negative()) {
            res = '-' + res;
        }
        return res;
    }

    bigint10 bigint10::read_from_string(std::string str, int base) {
        if (base > DIGIT_COUNT || base < 2) {
            throw std::logic_error(ERR_MSG);
        }
        if (str.size() < 1 || str == "-") {
            throw std::logic_error("Invalid string");
        }

        bigint10 curr, mult = base;
        int i = 0;
        bool neg = false;
        if (str[i] == '-') {
            neg = true;
            i++;
        }
        for (; i < str.size(); i++) {
            curr *= mult;
            int d = -1;
            if ('0' <= str[i] && str[i] <= '9') {
                d = str[i] - '0';
            }
            else if ('a' <= str[i] && str[i] <= 'z') {
                d = str[i] - 'a' + 10;
            }
            else if ('A' <= str[i] && str[i] <= 'Z') {
                d = str[i] - 'A' + 10;
            }
            if (0 <= d && d < base) {
                curr += d;
            }
            else {
                throw std::logic_error("Invalid character in number string");
            }
        }
        curr.negative = neg;
        return curr;
    }

    #undef ERR_MSG
    #undef DIGIT_COUNT
    #undef ERR_MSG2

    ostream& operator<<(ostream& os, const bigint10& obj) {
        string str;
        stringstream sstr;
        switch (os.flags() & os.basefield) {
        case ios_base::hex:
            str = obj.create_base_string(16, (os.flags() & os.uppercase) != 0);
            os << str;
            break;
        case ios_base::oct:
            str = obj.create_base_string(8, (os.flags() & os.uppercase) != 0);
            os << str;
            break;
        case ios_base::dec:
            if (obj.negative) {
                sstr << '-';
            }
            sstr << setbase(10) << obj.digits->back();
            for (int i = obj.digits->size() - 2; i >= 0; i--) {
                sstr << setw(9) << setfill('0') << obj.digits->at(i);
            }
            os << sstr.str();
            break;
        }
        return os;
    }

    istream& operator>>(istream& is, bigint10& obj) {
        string str;
        auto pos = is.tellg();
        if (!(is >> str)) return is;
        
        try {
            switch (is.flags() & is.basefield) {
            case ios_base::hex:
                obj = bigint10::read_from_string(str, 16);
                break;
            case ios_base::oct:
                obj = bigint10::read_from_string(str, 8);
                break;
            case ios_base::dec:
                obj = bigint10::read_from_string(str, 10);
                break;
            }
        }
        catch (logic_error& e) {
    #ifdef DEBUG
            cerr << e.what() << endl;
    #endif
            is.seekg(pos);
            is.setstate(ios::failbit);
        }

        return is;
    }

}
