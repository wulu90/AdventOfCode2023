#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

void part1() {
    ifstream input{"input"s};
    vector<string> gardenmap;

    for (string str; getline(input, str);) {
        gardenmap.push_back(str);
    }

    set<pair<size_t, size_t>> visited;
    queue<pair<size_t, size_t>> que;

    size_t sr, sc;    // S row,col
    for (sr = 0; sr < gardenmap.size(); ++sr) {
        sc = gardenmap[sr].find('S');
        if (sc != string::npos) {
            break;
        }
    }

    que.push({sr, sc});

    vector<vector<int>> dirs{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int step = 1; step <= 64; ++step) {
        auto qsize = que.size();
        for (size_t i = 0; i < qsize; ++i) {
            auto [row, col] = que.front();
            for (auto d : dirs) {
                auto r = row + d[0];
                auto c = col + d[1];

                if (r < gardenmap.size() && c < gardenmap[0].size() && gardenmap[r][c] != '#' && !visited.contains({r, c})) {
                    que.push({r, c});
                    visited.insert({r, c});
                }
            }
            que.pop();
        }
        visited.clear();
    }

    cout << que.size() << endl;
}

int main() {
    part1();
    return 0;
}