#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

int calc_location(const vector<string>& rocks, int r, int c) {
    int finalrow      = r;
    int cube          = -1;
    int rounded_count = 0;
    for (int i = r - 1; i >= 0; --i) {
        if (rocks[i][c] == '#') {
            cube = i;
            break;
        } else {
            if (rocks[i][c] == 'O') {
                ++rounded_count;
            }
        }
    }
    finalrow = cube + rounded_count + 1;
    return finalrow;
}

void part1() {
    ifstream input("input");
    vector<string> rocks;

    for (string str{}; getline(input, str);) {
        rocks.push_back(str);
    }

    int sum = 0;
    for (int i = 0; i < rocks.size(); ++i) {
        for (int j = 0; j < rocks.begin()->size(); ++j) {
            if (rocks[i][j] == 'O') {
                sum += rocks.size() - calc_location(rocks, i, j);
            }
        }
    }
    cout << sum << endl;
}

void tilt(string& str) {
    auto empty = string::npos;

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '.') {
            if (empty == string::npos) {
                empty = i;
            }

        } else if (str[i] == 'O') {
            if (empty != string::npos) {
                str[i]     = '.';
                str[empty] = 'O';
                ++empty;
            }
        } else {
            empty = string::npos;
        }
    }
}

void vertical(vector<string>& rocks, bool to_south) {
    for (int i = 0; i < rocks[0].size(); ++i) {
        string str(rocks.size(), ' ');
        for (int j = 0; j < rocks.size(); ++j) {
            str[j] = rocks[j][i];
        }
        if (to_south) {
            reverse(str.begin(), str.end());
        }
        tilt(str);

        if (to_south) {
            reverse(str.begin(), str.end());
        }
        for (int j = 0; j < rocks.size(); ++j) {
            rocks[j][i] = str[j];
        }
    }
}

void horizon(vector<string>& rocks, bool to_east) {
    for (int i = 0; i < rocks.size(); ++i) {
        if (to_east) {
            reverse(rocks[i].begin(), rocks[i].end());
        }
        tilt(rocks[i]);
        if (to_east) {
            reverse(rocks[i].begin(), rocks[i].end());
        }
    }
}

void cycle(vector<string>& rocks) {
    vertical(rocks, false);
    horizon(rocks, false);
    vertical(rocks, true);
    horizon(rocks, true);
}

int calc_north_load(const vector<string>& rocks) {
    int rowcount = rocks.size();
    int sum      = 0;
    for (size_t i = 0; i < rowcount; ++i) {
        sum += count(rocks[i].begin(), rocks[i].end(), 'O') * (rowcount - i);
    }
    return sum;
}

void part2() {
    ifstream input("input");
    vector<string> rocks;

    for (string str{}; getline(input, str);) {
        rocks.push_back(str);
    }

    map<vector<string>, size_t> state_inx;
    map<size_t, vector<string>> inx_state;
    size_t final_inx = 0;
    for (size_t i = 1; i <= 1000000000ull; ++i) {
        cycle(rocks);

        if (state_inx.contains(rocks)) {
            final_inx = state_inx[rocks] + (1000000000ull - i) % (i - state_inx[rocks]);
            break;
        } else {
            state_inx.insert({rocks, i});
            inx_state.insert({i, rocks});
        }
    }

    cout << calc_north_load(inx_state[final_inx]) << endl;
}

int main() {
    part1();
    part2();
    return 0;
}