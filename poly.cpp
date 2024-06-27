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

int main(int argc, char** argv) {
    istream* in = &cin;
    fstream f;
    if (argc > 1) {
        f.open(argv[1]);
        in = &f;
    }
    Context ctx(*in);
    ctx.run();
}
