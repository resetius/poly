#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <assert.h>

#include "poly.hpp"

using namespace std;
using namespace poly;

void test_term() {
    Term a{2, {0, 2}, {1,2}};
    cout << to_string(a) << "\n";
    assert(to_string(a) == "2a^1c^2");

    Term b{31337, {1, 2, 3}, {2,3,1}};
    cout << to_string(b) << "\n";
    assert(to_string(b) == "31337b^2c^3d^1");

    Term c = a*b;
    cout << to_string(c) << "\n";
    assert(to_string(c) == "62674a^1b^2c^5d^1");
}

void test_poly_plus_term() {
    Poly p = Poly{} + Term{2, {0, 2}, {1,2}};
    cout << to_string(p) << "\n";
    assert(to_string(p) == "2a^1c^2");
    p = p + Term{2, {0, 2}, {1,2}};
    assert(to_string(p) == "4a^1c^2");
    p = p + Term{31337, {1, 2, 3}, {2,3,1}};
    cout << to_string(p) << "\n";
    assert(to_string(p) == "4a^1c^2+31337b^2c^3d^1");
    p = p + Term{2, {0, 2}, {1,2}};
    assert(to_string(p) == "6a^1c^2+31337b^2c^3d^1");
}

void test_poly_plus_poly() {
    Poly p1 = Poly{} + Term{1, {0}, {1}} + Term{1, {1}, {1}} + Term{1, {2}, {1}};
    cout << to_string(p1) << "\n";
    Poly p2 = Poly{} + Term{1, {2}, {1}} + Term{1, {3}, {1}} + Term{1, {4}, {1}};
    cout << to_string(p2) << "\n";
    Poly p3 = p1+p2;
    cout << to_string(p3) << "\n";
    assert(to_string(p3) == "1a^1+1b^1+2c^1+1d^1+1e^1");
    Poly _const = Poly{} + Term{2};
    p3 = p1+p2+_const;
    cout << to_string(p3) << "\n";
    assert(to_string(p3) == "2+1a^1+1b^1+2c^1+1d^1+1e^1");
}

void test_poly_mul_poly() {
    Poly p1 = Poly{} + Term{1, {0}, {1}} + Term{1, {1}, {1}};
    Poly p2 = Poly{} + Term{1, {0}, {1}} + Term{-1, {1}, {1}};
    auto p = p1 * p1;
    cout << to_string(p) << "\n";
    assert(to_string(p) == "1a^2+2a^1b^1+1b^2");

    p = p1 * p2;
    cout << to_string(p) << "\n";
    assert(to_string(p) == "1a^2+-1b^2");

    Poly _const = Poly{} + Term{2};
    p = p1 * _const;
    cout << to_string(_const) << "\n";
    cout << to_string(p) << "\n";
    assert(to_string(p) == "2a^1+2b^1");
}

void test_poly_pow() {
    Poly p1 = Poly{} + Term{1, {0}, {1}} + Term{1, {1}, {1}};
    Poly p2 = Poly{} + Term{1, {0}, {1}} + Term{-1, {1}, {1}};

    auto p = p1^2;
    cout << to_string(p) << "\n";
    assert(to_string(p) == "1a^2+2a^1b^1+1b^2");
    p = p1^3;
    cout << to_string(p) << "\n";
    assert(to_string(p) == "1a^3+3a^1b^2+3a^2b^1+1b^3");
    p = p1^7;
    cout << to_string(p) << "\n";
    assert(to_string(p) == "1a^7+7a^1b^6+21a^2b^5+35a^3b^4+35a^4b^3+21a^5b^2+7a^6b^1+1b^7");
}

struct Context {
    istream& in;
    unordered_map<string, int> var2id;
    Poly poly;

    Context(istream& in)
        : in(in)
    { }

    void run() {
        poly = process();
        vector<pair<string,int>> vars;
        for (const auto& [k,v] : var2id) {
            vars.emplace_back(k, v);
        }
        vector<int> id2id(vars.size());
        sort(vars.begin(), vars.end());
        int i = 0;
        for (const auto& [_, id] : vars) {
            id2id[id] = i++;
        }
        vector<vector<i64>> rows;
        for (const auto& t : poly.terms) {
            vector<i64> row(vars.size()+1);
            row[vars.size()] = t.koef;
            for (int i = 0; i < t.ind.size(); i++) {
                auto id = id2id[t.ind[i]];
                row[id] = t.exp[i];
            }
            rows.emplace_back(std::move(row));
        }
        sort(rows.begin(), rows.end());
        for (const auto& r : rows) {
            for (int i = 0; i < r.size(); i++) {
                cout << r[i];
                if (i != r.size() - 1) {
                    cout << ",";
                }
            }
            cout << "\n";
        }
    }

    Poly process() {
        string token;
        in >> token;
        if (token == "+") {
            auto left = process();
            auto right = process();
            return left + right;
        } else if (token == "-") {
            auto left = process();
            auto right = process();
            return left + (right * (Poly{} + Term{-1}));
        } else if (token == "*") {
            auto left = process();
            auto right = process();
            return left * right;
        } else if (token == "^") {
            auto left = process();
            in >> token;
            return left ^ stoll(token);
        } else {
            // var
            if (isvar(token)) {
                auto it = var2id.find(token);
                int id = var2id.size();
                if (it == var2id.end()) {
                    var2id.emplace(token, id);
                } else {
                    id = it->second;
                }
                return Poly{} + Term{1, {id}, {1}};
            } else {
                auto koef = stoll(token);
                if (koef) {
                    return Poly{} + Term{stoll(token)};
                } else {
                    return Poly{};
                }
            }
        }
    }

    bool isvar(const string& token) {
        for (auto c : token) {
            if (!('a' <= c && c <= 'z')) {
                return false;
            }
        }
        return true;
    }
};

void test_all() {
    //test_term();
    //test_poly_plus_term();
    //test_poly_plus_poly();
    //test_poly_mul_poly();
    test_poly_pow();
}

int main(int argc, char** argv) {
    // test_all();
    istream* in = &cin;
    fstream f;
    if (argc > 1) {
        f.open(argv[1]);
        in = &f;
    }
    Context ctx(*in);
    ctx.run();
}
