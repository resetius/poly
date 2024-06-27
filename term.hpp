#pragma once

#include <vector>
#include <string>

#include "common.hpp"

namespace poly {

struct Term {
    i64 koef = 0;
    std::vector<int> ind = {};
    std::vector<u64> exp = {};

    bool operator<(const Term& other) const {
        if (ind == other.ind) {
            return exp < other.exp;
        } else {
            return ind < other.ind;
        }
    }
};

inline Term operator*(const Term& a, const Term& b) {
    int i,j;
    i = j = 0;
    if (!a.koef || !b.koef) {
        return {};
    }

    std::vector<int> ind;
    std::vector<u64> exp;
    while (i < a.exp.size() && j < b.exp.size()) {
        if (a.ind[i] == b.ind[j]) {
            ind.emplace_back(a.ind[i]);
            exp.emplace_back(a.exp[i] + b.exp[j]);
            i++; j++;
        } else if (a.ind[i] < b.ind[j]) {
            ind.emplace_back(a.ind[i]);
            exp.emplace_back(a.exp[i]);
            i++;
        } else { // if (a.ind[i] > b.ind[j]) {
            ind.emplace_back(b.ind[j]);
            exp.emplace_back(b.exp[j]);
            j++;
        }
    }
    for (; i < a.exp.size(); i++) {
        ind.emplace_back(a.ind[i]);
        exp.emplace_back(a.exp[i]);
    }
    for (; j < b.exp.size(); j++) {
        ind.emplace_back(b.ind[j]);
        exp.emplace_back(b.exp[j]);
    }

    return {a.koef * b.koef, std::move(ind), std::move(exp)};
}

inline std::string to_string(const Term& term) {
    std::string ans;
    ans += std::to_string(term.koef);
    int n = term.exp.size();
    for (int i = 0; i < n; i++) {
        ans += (term.ind[i]+'a');
        ans += '^';
        ans += std::to_string(term.exp[i]);
    }
    return ans;
}

}

