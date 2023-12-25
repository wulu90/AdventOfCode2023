#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <ranges>
#include <set>
#include <string>
#include <string_view>

using namespace std;

// https://en.wikipedia.org/wiki/Karger%27s_algorithm
// https://reference.wolfram.com/language/ref/FindMinimumCut.html

// too many string copy , may be slow?
pair<string, string> random_choice(map<string, multiset<string>>& adjmap) {
    random_device rd;
    uniform_int_distribution<> dist(0, adjmap.size() - 1);
    auto n1 = dist(rd);
    auto be = adjmap.begin();
    for (int i = 0; i < n1; ++i) {
        ++be;
    }
    auto k = be->first;
    uniform_int_distribution<> dist2(0, be->second.size() - 1);
    auto n2   = dist2(rd);
    auto vabe = be->second.begin();
    for (int i = 0; i < n2; ++i) {
        ++vabe;
    }
    auto v = *vabe;

    // cout << n1 << ' ' << n2 << endl;

    return {k, v};
}

void kargers(map<string, multiset<string>> adjmap) {
    while (adjmap.size() > 2) {
        auto [k, v]  = random_choice(adjmap);
        auto newnode = k + v;
        adjmap.insert({newnode, {}});
        for (auto n : adjmap[k]) {
            adjmap[newnode].insert(n);
            adjmap[n].erase(k);
            adjmap[n].insert(newnode);
        }
        adjmap.erase(k);
        for (auto n : adjmap[v]) {
            adjmap[newnode].insert(n);
            adjmap[n].erase(v);
            adjmap[n].insert(newnode);
        }
        adjmap.erase(v);

        // remove self loop
        adjmap[newnode].erase(newnode);
    }

    if (adjmap.begin()->second.size() == 3) {
        // cout << adjmap.begin()->first << endl;
        cout << adjmap.begin()->first.size() * adjmap.rbegin()->first.size() / 9 << endl;
    }
}

void part1() {
    ifstream input{"input"s};
    map<string, multiset<string>> adjmap;
    for (string str; getline(input, str);) {
        auto strsp = str | views::split(": "sv);
        string name{string_view{*strsp.begin()}};
        adjmap.insert({name, {}});
        for (auto sp : string_view{*++strsp.begin()} | views::split(" "sv)) {
            adjmap[name].insert(string{string_view{sp}});
        }
        for (auto& n : adjmap[name]) {
            adjmap[n].insert(name);
        }
    }

    int nv  = adjmap.size();
    int num = nv * nv * static_cast<int>(ceil(log(nv)));

    for (int i = 0; i < num; ++i) {
        kargers(adjmap);
    }
}

int main() {
    part1();
    return 0;
}