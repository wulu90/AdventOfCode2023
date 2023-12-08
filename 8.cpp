#include <algorithm>
#include <concepts>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>
using namespace std;

void part1() {
    ifstream input("input");
    string instructions;
    getline(input, instructions);

    string str;
    getline(input, str);

    map<string, pair<string, string>> maps;
    while (getline(input, str)) {
        maps.insert({{str.begin(), str.begin() + 3}, {{str.begin() + 7, str.begin() + 10}, {str.begin() + 12, str.begin() + 15}}});
    }

    int step = 0;

    string node{"AAA"};

    while (node != "ZZZ"s) {
        if (instructions[step % instructions.size()] == 'L') {
            node = maps[node].first;
        } else {
            node = maps[node].second;
        }
        ++step;
    }

    cout << step << endl;
}

void part2() {
    ifstream input("input");
    string instructions;
    getline(input, instructions);

    string str;
    getline(input, str);

    map<string, pair<string, string>> maps;
    while (getline(input, str)) {
        maps.insert({{str.begin(), str.begin() + 3}, {{str.begin() + 7, str.begin() + 10}, {str.begin() + 12, str.begin() + 15}}});
    }

    vector<string> nodevec;

    for (auto& [s, v] : maps) {
        if (s.ends_with('A')) {
            nodevec.push_back(s);
        }
    }
    // int step = 0;
    // while (!all_of(nodevec.begin(), nodevec.end(), [](const string& s) { return s.ends_with('Z'); })) {
    //     vector<string> tmp;
    //     for (auto s : nodevec) {
    //         tmp.push_back(instructions[step % instructions.size()] == 'L' ? maps[s].first : maps[s].second);
    //     }
    //     nodevec = tmp;
    //     ++step;
    // }

    vector<int64_t> stepvec;
    for (auto s : nodevec) {
        int step = 0;

        while (!s.ends_with('Z')) {
            s = instructions[step % instructions.size()] == 'L' ? maps[s].first : maps[s].second;
            ++step;
        }
        stepvec.push_back(step);
    }

    cout << accumulate(stepvec.begin(), stepvec.end(), 1LL, [](const int64_t a, const int64_t b) { return lcm(a, b); }) << endl;
}

int main() {
    part1();
    part2();
    return 0;
}