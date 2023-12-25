#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> dirs{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

void backtracking(size_t endinx, size_t curr, vector<bool>& visited, size_t step, size_t& maxstep, size_t rownum, size_t colnum,
                  const vector<string>& hikingmap) {
    if (curr == endinx) {
        maxstep = max(maxstep, step);
        return;
    } else {
        auto row = curr / colnum;
        auto col = curr % colnum;
        for (auto& d : dirs) {
            auto r = row + d[0];
            auto c = col + d[1];

            if (r < rownum && c < colnum && !visited[r * colnum + c] && hikingmap[r][c] != '#') {
                auto ch = hikingmap[r][c];
                if (ch == '.') {
                    visited[r * colnum + c] = true;
                    backtracking(endinx, r * colnum + c, visited, step + 1, maxstep, rownum, colnum, hikingmap);
                    visited[r * colnum + c] = false;
                } else if (ch == '>' && c != col - 1) {
                    visited[r * colnum + c]     = true;
                    visited[r * colnum + c + 1] = true;
                    backtracking(endinx, r * colnum + c + 1, visited, step + 2, maxstep, rownum, colnum, hikingmap);
                    visited[r * colnum + c]     = false;
                    visited[r * colnum + c + 1] = false;
                } else if (ch == 'v' && r != row - 1) {
                    visited[r * colnum + c]       = true;
                    visited[(r + 1) * colnum + c] = true;
                    backtracking(endinx, (r + 1) * colnum + c, visited, step + 2, maxstep, rownum, colnum, hikingmap);
                    visited[r * colnum + c]       = false;
                    visited[(r + 1) * colnum + c] = false;
                }
            }
        }
    }
}

void part1() {
    ifstream input{"input"s};
    vector<string> hikingmap;
    for (string str; getline(input, str);) {
        hikingmap.push_back(str);
    }

    auto rownum = hikingmap.size();
    auto colnum = hikingmap.begin()->size();

    auto startinx = hikingmap.begin()->find('.');
    auto endinx   = (rownum - 1) * colnum + hikingmap.back().find('.');

    vector<bool> visited(rownum * colnum, false);
    size_t maxstep    = 0;
    visited[startinx] = true;
    backtracking(endinx, startinx, visited, 0, maxstep, rownum, colnum, hikingmap);

    cout << maxstep << endl;
}

void backtracking2(size_t endinx, size_t curr, vector<bool>& visited, const map<size_t, map<size_t, size_t>>& adjmap, size_t& maxstep,
                   size_t step) {
    if (curr == endinx) {
        maxstep = max(maxstep, step);
        return;
    } else {
        for (auto [next, num] : adjmap.at(curr)) {
            if (!visited[next]) {
                visited[next] = true;
                backtracking2(endinx, next, visited, adjmap, maxstep, step + num);
                visited[next] = false;
            }
        }
    }
}

void part2() {
    ifstream input{"input"s};
    vector<string> hikingmap;
    for (string str; getline(input, str);) {
        hikingmap.push_back(str);
    }

    auto rownum = hikingmap.size();
    auto colnum = hikingmap.begin()->size();

    auto startinx = hikingmap.begin()->find('.');
    auto endinx   = (rownum - 1) * colnum + hikingmap.back().find('.');

    set<size_t> nodeset;    // position can connect to more than 2 others , include start and end;
    nodeset.insert(startinx);
    nodeset.insert(endinx);

    for (size_t i = 0; i < rownum; ++i) {
        for (size_t j = 0; j < colnum; ++j) {
            if (hikingmap[i][j] == '#') {
                continue;
            }
            int conn = 0;
            for (auto& d : dirs) {
                auto r = i + d[0];
                auto c = j + d[1];
                if (r < rownum && c < colnum && hikingmap[r][c] != '#') {
                    ++conn;
                }
            }
            if (conn > 2) {
                nodeset.insert(i * colnum + j);
            }
        }
    }

    map<size_t, map<size_t, size_t>> adjmap;
    for (auto i : nodeset) {
        for (auto& d : dirs) {
            auto r = i / colnum + d[0];
            auto c = i % colnum + d[1];

            if (r < rownum && c < colnum && hikingmap[r][c] != '#') {
                auto curr         = r * colnum + c;
                auto prev         = i;
                auto step         = 1;
                bool findnextnode = false;
                while (true) {
                    for (auto& di : dirs) {
                        auto ri = curr / colnum + di[0];
                        auto ci = curr % colnum + di[1];
                        if (ri < rownum && ci < colnum && hikingmap[ri][ci] != '#' && ri * colnum + ci != prev) {
                            prev = curr;
                            curr = ri * colnum + ci;
                            ++step;
                        }
                        if (nodeset.contains(curr)) {
                            findnextnode = true;
                            break;
                        }
                    }
                    if (findnextnode) {
                        adjmap[i].insert({curr, step});
                        break;
                    }
                }
            }
        }
    }

    vector<bool> visited(rownum * colnum, false);
    visited[startinx] = true;
    size_t maxstep    = 0;

    backtracking2(endinx, startinx, visited, adjmap, maxstep, 0);

    cout << maxstep << endl;
}

int main() {
    part1();
    part2();
    return 0;
}