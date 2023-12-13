#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

// int line(const vector<string>& p) {
//     map<string, vector<int>> row_line_indexs;
//     map<string, vector<int>> col_line_indexs;

//     for (int i = 0; i < p.size(); ++i) {
//         row_line_indexs[p[i]].push_back(i);
//     }

//     for (int i = 0; i < p[0].size(); ++i) {
//         string str;
//         for (int j = 0; j < p.size(); ++j) {
//             str.push_back(p[j][i]);
//         }
//         col_line_indexs[str].push_back(i);
//     }
// }

bool check_line(const vector<string>& pattern, size_t line) {
    bool res = true;

    auto steps = min(line, pattern.size() - line);

    for (size_t i = 0; i < steps; ++i) {
        if (pattern[line - 1 - i] != pattern[line + i]) {
            res = false;
            break;
        }
    }
    return res;
}

size_t find_reflection(const vector<string>& pattern) {
    size_t res = 0;
    for (size_t i = 1; i < pattern.size(); ++i) {
        if (check_line(pattern, i)) {
            res = i;
            break;
        }
    }
    return res;
}

vector<string> to_col(const vector<string>& pattern) {
    vector<string> vec(pattern[0].size(), string(pattern.size(), ' '));
    for (auto i = 0; i < pattern[0].size(); ++i) {
        for (auto j = 0; j < pattern.size(); ++j) {
            vec[i][j] = pattern[j][i];
        }
    }
    return vec;
}

void print_horizon_line(const vector<string>& p, size_t line, ofstream& output) {
    for (size_t i = 0; i < p.size(); ++i) {
        output << i + 1 << "  " << p[i] << endl;
        if (i == line - 1) {
            output << "---------------------------------" << endl;
        }
    }
    output << endl;
}

void print_vertical_line(const vector<string>& p, size_t line, ofstream& output) {
    for (size_t i = 0; i < p.size(); ++i) {
        for (size_t j = 0; j < p[0].size(); ++j) {
            output << p[i][j];
            if (j == line - 1) {
                output << "|";
            }
        }
        output << endl;
    }
    output << endl;
}

void part1() {
    ifstream input("input");
    // ofstream output("output.txt");
    string str;
    vector<vector<string>> all;
    vector<string> sv;
    while (input.good()) {
        getline(input, str);
        if (str.empty() || input.eof()) {
            all.push_back(sv);
            sv.clear();
        } else {
            sv.push_back(str);
        }
    }

    uint64_t sum = 0;

    for (auto& p : all) {
        size_t h;
        h = find_reflection(p);
        if (h != 0) {
            sum += h * 100;
            // print_horizon_line(p, h, output);
        }
        h = find_reflection(to_col(p));
        if (h != 0) {
            sum += h;
            // print_vertical_line(p, h, output);
        }
    }

    cout << sum << endl;
}

size_t hamming_distance(const string& s1, const string& s2) {
    size_t count = 0;
    for (size_t i = 0; i < s1.size(); ++i) {
        if (s1[i] != s2[i]) {
            ++count;
        }
    }
    return count;
}

size_t sum_distance(const vector<string>& pattern, size_t line) {
    size_t sum_dist = 0;
    auto steps      = min(line, pattern.size() - line);

    for (size_t i = 0; i < steps; ++i) {
        sum_dist += hamming_distance(pattern[line - 1 - i], pattern[line + i]);
    }
    return sum_dist;
}

size_t reflection_dist_one(const vector<string>& pattern) {
    size_t res = 0;
    for (size_t i = 1; i < pattern.size(); ++i) {
        if (sum_distance(pattern, i) == 1) {
            res = i;
            break;
        }
    }
    return res;
}

void part2() {
    ifstream input("input");
    string str;
    vector<vector<string>> all;
    vector<string> sv;
    while (input.good()) {
        getline(input, str);
        if (str.empty() || input.eof()) {
            all.push_back(sv);
            sv.clear();
        } else {
            sv.push_back(str);
        }
    }

    uint64_t sum = 0;

    for (auto& p : all) {
        size_t h;
        h = reflection_dist_one(p);
        if (h != 0) {
            sum += h * 100;
        }
        h = reflection_dist_one(to_col(p));
        if (h != 0) {
            sum += h;
        }
    }

    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}