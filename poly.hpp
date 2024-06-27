#include "term.hpp"

namespace poly {

struct Poly {
    // assume sorted by {ind,exp}
    std::vector<Term> terms = {};
};

inline std::string to_string(const Poly& poly) {
    int n = poly.terms.size();
    std::string ans;
    for (int i = 0; i < n; i++) {
        ans += to_string(poly.terms[i]);
        if (i != n-1) {
            ans += '+';
        }
    }
    return ans;
}

inline Poly operator+(const Poly& a, const Poly& b) {
    int i,j;
    i = 0; j = 0;
    std::vector<Term> terms;
    while (i < a.terms.size() && j < b.terms.size()) {
        if (!(a.terms[i] < b.terms[j]) && !(b.terms[j] < a.terms[i])) {
            if (a.terms[i].koef + b.terms[j].koef) {
                terms.emplace_back(Term{a.terms[i].koef + b.terms[j].koef, a.terms[i].ind, a.terms[i].exp});
            }
            i++; j++;
        } else if (a.terms[i] < b.terms[j]) {
            terms.emplace_back(a.terms[i]);
            i++;
        } else { // if a.terms[i] > b.terms[j]
            terms.emplace_back(b.terms[j]);
            j++;
        }
    }
    for (; i < a.terms.size(); i++) {
        terms.emplace_back(a.terms[i]);
    }
    for (; j < b.terms.size(); j++) {
        terms.emplace_back(b.terms[j]);
    }
    return {terms};
}

inline Poly operator+(const Poly& a, const Term& b) {
    return a + Poly{{b}};
}

inline Poly operator*(const Poly& a, const Poly& b) {
    if (a.terms.size() < b.terms.size()) {
        return b*a;
    }

    std::vector<Poly> pols; pols.reserve(b.terms.size());
    // O(n^2 log(n)) !
    for (int i = 0; i < b.terms.size(); i++) {
        std::vector<Term> terms(a.terms.size());
        for (int j = 0; j < a.terms.size(); j++) {
            terms[j] = a.terms[j] * b.terms[i];
        }
        std::sort(terms.begin(), terms.end());
        if (!terms.empty()) {
            pols.push_back(Poly{terms});
            // pols.emplace_back(std::move(terms)); // fails on gcc-12
        }
    }
    Poly p;
    for (const auto& pp : pols) {
        p = p + pp;
    }

    return p;
}

inline Poly operator^(const Poly& a, i64 p) {
    if (p == 0) {
        return Poly{} + Term{1};
    }
    if (p == 1) {
        return a;
    }
    auto half = a^(p/2);
    if (p % 2 == 0) {
        return half * half;
    } else {
        return half * half * a;
    }
}

}

