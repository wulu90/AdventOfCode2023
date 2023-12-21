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
    ifstream input{"input"s};
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

    for (int step = 1; step <= 64; ++step) {
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

    cout << visited.size() << endl;
}

vector<uint64_t> triple(const vector<string>& gardenmap, const pair<size_t, size_t>& start) {
    vector<uint64_t> res(8, 0ull);

    uint64_t a = 64;
    uint64_t b = 130;
    uint64_t c = 130 + 131;
    uint64_t d = 130 + 131 * 2;
    uint64_t e = 64 + 131;
    uint64_t f = 64 + 131 * 2;
    uint64_t g = 64 + 131 * 3;
    uint64_t h = 65 + 131 * 2;

    auto [sr, sc] = start;
    set<pair<size_t, size_t>> visited;
    visited.insert({sr, sc});
    vector<vector<int>> dirs{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (size_t i = 1; i <= g; ++i) {
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

        if (i == a) {
            res[0] = visited.size();
        }
        if (i == b) {
            res[1] = visited.size();
        }
        if (i == c) {
            res[2] = visited.size();
        }
        if (i == d) {
            res[3] = visited.size();
        }
        if (i == e) {
            res[4] = visited.size();
        }
        if (i == f) {
            res[5] = visited.size();
        }
        if (i == g) {
            res[6] = visited.size();
        }
        if (i == h) {
            res[7] = visited.size();
        }
    }
    return res;
}

// 26501365=65+131*202300
// in notebook !
void part2() {
    ifstream input{"input"s};
    vector<string> gardenmap;

    for (string str; getline(input, str);) {
        gardenmap.push_back(str);
    }

    vector<vector<int>> dirs{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    size_t sr, sc;    // S row,col
    for (sr = 0; sr < gardenmap.size(); ++sr) {
        sc = gardenmap[sr].find('S');
        if (sc != string::npos) {
            break;
        }
    }

    // 0 top-left, 1 top-middle, 2 top-right, 3 middle-right, 4 bottom-right, 5 bottom-middle, 6 bottom-left, 7 middle-left
    vector<pair<size_t, size_t>> startpoints{{0, 0}, {0, 65}, {0, 130}, {65, 130}, {130, 130}, {130, 65}, {130, 0}, {65, 0}};
    startpoints.push_back({sr, sc});
    vector<vector<uint64_t>> allsit;

    for (auto& sp : startpoints) {
        allsit.push_back(triple(gardenmap, sp));
    }

    uint64_t sum = 0;

    uint64_t k = 202300;

    sum += allsit[5][1];
    sum += allsit[5][2] * (k / 2) + allsit[5][3] * (k / 2 - 1);
    sum += allsit[4][0] * k;
    sum += allsit[4][4] * (k - 1);

    sum += allsit[3][1];
    sum += allsit[3][2] * (k / 2) + allsit[3][3] * (k / 2 - 1);
    sum += allsit[2][0] * k;
    sum += allsit[2][4] * (k - 1);

    sum += allsit[1][1];
    sum += allsit[1][2] * (k / 2) + allsit[1][3] * (k / 2 - 1);
    sum += allsit[0][0] * k;
    sum += allsit[0][4] * (k - 1);

    sum += allsit[7][1];
    sum += allsit[7][2] * (k / 2) + allsit[7][3] * (k / 2 - 1);
    sum += allsit[6][0] * k;
    sum += allsit[6][4] * (k - 1);

    sum += allsit[4][5] * (k * (k - 2) / 4) + allsit[4][6] * ((k - 2) * (k - 2) / 4);
    sum += allsit[2][5] * (k * (k - 2) / 4) + allsit[2][6] * ((k - 2) * (k - 2) / 4);
    sum += allsit[0][5] * (k * (k - 2) / 4) + allsit[0][6] * ((k - 2) * (k - 2) / 4);
    sum += allsit[6][5] * (k * (k - 2) / 4) + allsit[6][6] * ((k - 2) * (k - 2) / 4);

    sum += allsit[8][7];
    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}