#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

void part1() {
    ifstream input{"input_test"s};
    ofstream output{"test_output.txt"s};
    vector<string> gardenmap;

    for (string str; getline(input, str);) {
        gardenmap.push_back(str);
    }

    size_t sr, sc;    // S row,col
    for (sr = 0; sr < gardenmap.size(); ++sr) {
        sc = gardenmap[sr].find('S');
        if (sc != string::npos) {
            break;
        }
    }

    set<pair<size_t, size_t>> visited;
    visited.insert({sr, sc});
    vector<vector<int>> dirs{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int step = 1; step <= 131; ++step) {
        set<pair<size_t, size_t>> tmpset;
        for (auto [row, col] : visited) {
            for (auto d : dirs) {
                auto r = row + d[0];
                auto c = col + d[1];

                if (r < gardenmap.size() && c < gardenmap[0].size() && gardenmap[r][c] != '#' && !visited.contains({r, c})) {
                    tmpset.insert({r, c});
                }
            }
        }
        visited = move(tmpset);
    }

    for (size_t i = 0; i < gardenmap.size(); ++i) {
        for (size_t j = 0; j < gardenmap[0].size(); ++j) {
            if (visited.contains({i, j})) {
                output << 'O';
            } else {
                output << gardenmap[i][j];
            }
        }
        output << '\n';
    }

    cout << visited.size() << endl;
}

int main() {
    part1();
    return 0;
}