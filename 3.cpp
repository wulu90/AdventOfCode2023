#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

bool is_engine_part(const vector<string>& engine, int row, int col_s, int col_e) {
    vector<pair<int, int>> neighbours;
    for (int i = col_s - 1; i <= col_e + 1; ++i) {
        neighbours.push_back({row - 1, i});
        neighbours.push_back({row + 1, i});
    }
    neighbours.push_back({row, col_s - 1});
    neighbours.push_back({row, col_e + 1});

    bool res = false;
    for (auto [r, c] : neighbours) {
        if (r >= 0 && r < engine.size() && c >= 0 && c < engine[0].size() && engine[r][c] != '.') {
            res = true;
            break;
        }
    }

    return res;
}

void part1() {
    fstream input{"input"};
    string str;

    vector<string> engine;

    while (getline(input, str)) {
        engine.push_back(str);
    }
    int sum = 0;
    for (int i = 0; i < engine.size(); ++i) {
        for (int j = 0; j < engine[0].size(); ++j) {
            if (isdigit(engine[i][j])) {
                int k   = j;
                int num = engine[i][j] - '0';
                while (isdigit(engine[i][++k])) {
                    num = num * 10 + engine[i][k] - '0';
                }

                if (is_engine_part(engine, i, j, k - 1)) {
                    sum += num;
                }

                j = k - 1;
            }
        }
    }

    cout << sum << endl;
}

void gear_part(const vector<string>& engine, int row, int col_s, int col_e, int num, map<pair<int, int>, vector<int>>& gear_adj) {
    vector<pair<int, int>> neighbours;
    for (int i = col_s - 1; i <= col_e + 1; ++i) {
        neighbours.push_back({row - 1, i});
        neighbours.push_back({row + 1, i});
    }
    neighbours.push_back({row, col_s - 1});
    neighbours.push_back({row, col_e + 1});

    for (auto [r, c] : neighbours) {
        if (r >= 0 && r < engine.size() && c >= 0 && c < engine[0].size() && engine[r][c] == '*') {
            gear_adj[{r, c}].push_back(num);
            break;
        }
    }
}

void part2() {
    fstream input{"input"};
    string str;

    vector<string> engine;

    while (getline(input, str)) {
        engine.push_back(str);
    }
    int sum = 0;
    map<pair<int, int>, vector<int>> gear_adj;
    for (int i = 0; i < engine.size(); ++i) {
        for (int j = 0; j < engine[0].size(); ++j) {
            if (isdigit(engine[i][j])) {
                int k   = j;
                int num = engine[i][j] - '0';
                while (isdigit(engine[i][++k])) {
                    num = num * 10 + engine[i][k] - '0';
                }

                gear_part(engine, i, j, k - 1, num, gear_adj);

                j = k - 1;
            }
        }
    }

    for (auto [_, v] : gear_adj) {
        if (v.size() > 1) {
            sum += v[0] * v[1];
        }
    }

    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}