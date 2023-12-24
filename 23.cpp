#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> dirs{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

void backtracking(size_t endinx, size_t curr, vector<bool>& visited, vector<size_t>& path, vector<vector<size_t>>& allpath, size_t rownum,
                  size_t colnum, const vector<string>& hikingmap) {
    // visited[curr]=true;
    // path.push_back(curr);

    if (curr == endinx) {
        allpath.push_back(path);
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
                    path.push_back(r * colnum + c);
                    visited[r * colnum + c] = true;
                    backtracking(endinx, r * colnum + c, visited, path, allpath, rownum, colnum, hikingmap);
                    path.pop_back();
                    visited[r * colnum + c] = false;
                } else if (ch == '>' && c != col - 1) {
                    path.push_back(r * colnum + c);
                    path.push_back(r * colnum + c + 1);
                    visited[r * colnum + c]     = true;
                    visited[r * colnum + c + 1] = true;
                    backtracking(endinx, r * colnum + c + 1, visited, path, allpath, rownum, colnum, hikingmap);
                    path.pop_back();
                    path.pop_back();
                    visited[r * colnum + c]     = false;
                    visited[r * colnum + c + 1] = false;
                } else if (ch == 'v' && r != row - 1) {
                    path.push_back(r * colnum + c);
                    path.push_back((r + 1) * colnum + c);
                    visited[r * colnum + c]       = true;
                    visited[(r + 1) * colnum + c] = true;
                    backtracking(endinx, (r + 1) * colnum + c, visited, path, allpath, rownum, colnum, hikingmap);
                    path.pop_back();
                    path.pop_back();
                    visited[r * colnum + c]       = false;
                    visited[(r + 1) * colnum + c] = false;
                }
            }
        }
    }
}

void backtracking_part2(size_t endinx, size_t curr, vector<bool>& visited, size_t& maxstep, size_t step, size_t rownum, size_t colnum,
                        const vector<string>& hikingmap) {
    // visited[curr]=true;
    // path.push_back(curr);

    if (curr == endinx) {
        // allpath.push_back(path);
        maxstep = max(maxstep, step);
        return;
    } else {
        auto row = curr / colnum;
        auto col = curr % colnum;
        for (auto& d : dirs) {
            auto r = row + d[0];
            auto c = col + d[1];

            if (r < rownum && c < colnum && !visited[r * colnum + c] && hikingmap[r][c] != '#') {
                // path.push_back(r * colnum + c);
                visited[r * colnum + c] = true;
                backtracking_part2(endinx, r * colnum + c, visited, maxstep, step + 1, rownum, colnum, hikingmap);
                // path.pop_back();
                visited[r * colnum + c] = false;
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
    auto endinx   = (rownum - 1) * colnum + hikingmap.rbegin()->find('.');

    vector<vector<size_t>> allpath;
    vector<size_t> path;
    vector<bool> visited(rownum * colnum, false);

    backtracking(endinx, startinx, visited, path, allpath, rownum, colnum, hikingmap);

    size_t maxstep = 0;
    for_each(allpath.begin(), allpath.end(), [&maxstep](auto it) { maxstep = max(maxstep, it.size()); });

    cout << maxstep << endl;
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
    auto endinx   = (rownum - 1) * colnum + hikingmap.rbegin()->find('.');

    vector<vector<size_t>> allpath;
    vector<size_t> path;
    vector<bool> visited(rownum * colnum, false);

    size_t maxstep = 0;

    backtracking_part2(endinx, startinx, visited, maxstep, 0, rownum, colnum, hikingmap);

    // size_t maxstep = 0;
    // for_each(allpath.begin(), allpath.end(), [&maxstep](auto it) { maxstep = max(maxstep, it.size()); });

    cout << maxstep << endl;
}

int main() {
    // part1();
    part2();
    return 0;
}