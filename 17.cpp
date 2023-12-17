#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
using namespace std;

enum class direction { up, left, down, right };
vector<vector<int64_t>> dirs{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

struct node {
    size_t inx;
    direction dire;
    uint8_t step;
    uint32_t lose;
};

vector<node> next_pos(const node& curr, const vector<uint8_t>& blocks, size_t rownum, size_t colnum) {
    vector<node> res;
    vector<size_t> turns{(static_cast<size_t>(curr.dire) + 4 - 1) % 4, (static_cast<size_t>(curr.dire) + 4 + 1) % 4};
    auto row = curr.inx / rownum;
    auto col = curr.inx % rownum;

    for (auto& ti : turns) {
        auto r = row + dirs[ti][0];
        auto c = col + dirs[ti][1];
        if (r < rownum && c < colnum) {
            res.emplace_back(r * rownum + c, static_cast<direction>(ti), 1, curr.lose + blocks[r * rownum + c]);
        }
    }
    if (curr.step < 3) {
        auto r = row + dirs[static_cast<size_t>(curr.dire)][0];
        auto c = col + dirs[static_cast<size_t>(curr.dire)][1];
        if (r < rownum && c < colnum) {
            res.emplace_back(r * rownum + c, curr.dire, curr.step + 1, curr.lose + blocks[r * rownum + c]);
        }
    }
    return res;
}

void part1() {
    ifstream input("input");
    vector<uint8_t> blocks;
    size_t colnum = 0;
    for (string str; getline(input, str);) {
        colnum = str.size();
        for (auto& c : str) {
            blocks.push_back(c - '0');
        }
    }

    auto rownum = blocks.size() / colnum;

    auto inf = numeric_limits<uint64_t>::max();

    vector<uint64_t> lose(rownum * colnum, inf);
    vector<bool> visited(rownum * colnum, false);
    vector<pair<direction, size_t>> dir_step(rownum * colnum);

    lose[0]     = 0;
    size_t curr = 0;

    vector<map<pair<direction, size_t>, size_t>> dsl(blocks.size());    // direction,steps,lose;

    dsl[1]      = {{{direction::right, 1}, blocks[1]}};
    dsl[colnum] = {{{direction::down, 1}, blocks[colnum]}};

    queue<node> nq;
    nq.push({1, direction::right, 1, blocks[1]});
    nq.push({colnum, direction::down, 1, blocks[colnum]});

    uint32_t minv = numeric_limits<uint32_t>::max();

    while (!nq.empty()) {
        auto qnum = nq.size();
        for (size_t i = 0; i < qnum; ++i) {
            auto& curr = nq.front();

            if (curr.inx == blocks.size() - 1) {
                minv = min(curr.lose, minv);
                nq.pop();
                continue;
            }

            for (auto& n : next_pos(curr, blocks, rownum, colnum)) {
                if (!dsl[n.inx].contains({n.dire, n.step}) ||
                    (dsl[n.inx].contains({n.dire, n.step}) && dsl[n.inx][{n.dire, n.step}] > n.lose)) {
                    nq.push(n);
                    dsl[n.inx][{n.dire, n.step}] = n.lose;
                }
            }
            nq.pop();
        }
    }

    cout << minv << endl;
}

int main() {
    part1();
    return 0;
}