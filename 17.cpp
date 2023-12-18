#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
using namespace std;

enum class direction { up, left, down, right };
vector<vector<int>> dirsum{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

struct state {
    int dist;
    int inx;
    direction dire;
    int step;
};

struct vertex {
    int inx;
    direction dire;
    int step;
};

auto operator<=>(const vertex& lhs, const vertex& rhs) {
    return tie(lhs.inx, lhs.dire, lhs.step) <=> tie(rhs.inx, rhs.dire, rhs.step);
}

direction turnleft(const direction& d) {
    direction res;
    using enum direction;
    switch (d) {
    case up:
        res = left;
        break;
    case left:
        res = down;
        break;
    case down:
        res = right;
        break;
    case right:
        res = up;
        break;
    }
    return res;
}

direction turnright(const direction& d) {
    using enum direction;
    switch (d) {
    case up:
        return right;
    case right:
        return down;
    case down:
        return left;
    default:
        return up;
    }
}

vector<pair<int, vertex>> nextstate(const vector<int>& blocks, const vertex& v, int rownum, int colnum, int mini, int maxi) {
    auto row = v.inx / rownum;
    auto col = v.inx % rownum;
    vector<pair<int, vertex>> res;
    if (v.step >= mini) {
        vector<direction> lr{turnleft(v.dire), turnright(v.dire)};
        for (auto d : lr) {
            auto r = row + dirsum[static_cast<size_t>(d)][0];
            auto c = col + dirsum[static_cast<size_t>(d)][1];
            if (r >= 0 && r < rownum && c >= 0 && c < colnum) {
                res.emplace_back(blocks[r * rownum + c], vertex{r * rownum + c, d, 1});
            }
        }
    }
    if (v.step < maxi) {
        auto cost = 0;
        for (int i = 1; i <= maxi - v.step; ++i) {
            auto r = row + dirsum[static_cast<size_t>(v.dire)][0] * i;
            auto c = col + dirsum[static_cast<size_t>(v.dire)][1] * i;

            if (r >= 0 && r < rownum && c >= 0 && c < colnum) {
                cost += blocks[r * rownum + c];
                res.emplace_back(cost, vertex{r * rownum + c, v.dire, v.step + i});
            }
        }
    }

    return res;
}

map<vertex, int> all_vertex(const vector<int>& blocks, int rownum, int colnum, int mini, int maxi) {
    map<vertex, int> res;
    for (size_t i = 0; i < rownum; ++i) {
        for (size_t j = 0; j < colnum; ++j) {
            for (int k = 0; k < 4; ++k) {
                for (int s = 1; s <= maxi; ++s) {
                    int r = i + dirsum[k][0] * s;
                    int c = j + dirsum[k][1] * s;
                    if (r >= 0 && r < rownum && c >= 0 && c < colnum) {
                        res.insert({vertex{r * rownum + c, direction{k}, s}, numeric_limits<int>::max()});
                    }
                }
            }
        }
    }
    return res;
}

int dijkstra_priority_queue(const vector<int>& blocks, const vertex& startv, int startcost, int rownum, int colnum, int mini, int maxi) {
    priority_queue<pair<int, vertex>, vector<pair<int, vertex>>, greater<pair<int, vertex>>> pq;
    set<vertex> seen;

    map<vertex, int> distance = all_vertex(blocks, rownum, colnum, mini, maxi);
    distance[startv]          = startcost;

    vertex curr{startv};

    while (true) {
        for (auto [nc, nv] : nextstate(blocks, curr, rownum, colnum, mini, maxi)) {
            if (!seen.contains(nv) && distance[nv] > distance[curr] + nc) {
                distance[nv] = distance[curr] + nc;
                pq.emplace(distance[nv], nv);
            }
        }

        seen.insert(curr);

        if (curr.inx == rownum * colnum - 1) {
            break;
        } else {
            curr = pq.top().second;
            pq.pop();
        }
    }

    return distance[curr];
}

void solve() {
    ifstream input("input");
    vector<int> blocks;
    int colnum = 0;
    for (string str; getline(input, str);) {
        colnum = str.size();
        for (auto& c : str) {
            blocks.push_back(c - '0');
        }
    }

    int rownum = blocks.size() / colnum;

    auto min1 = dijkstra_priority_queue(blocks, vertex{1, direction::right, 1}, blocks[1], rownum, colnum, 1, 3);
    auto min2 = dijkstra_priority_queue(blocks, vertex{colnum, direction::down, 1}, blocks[colnum], rownum, colnum, 1, 3);

    cout << min(min1, min2) << endl;

    auto min3 = dijkstra_priority_queue(blocks, vertex{1, direction::right, 1}, blocks[1], rownum, colnum, 4, 10);
    auto min4 = dijkstra_priority_queue(blocks, vertex{colnum, direction::down, 1}, blocks[colnum], rownum, colnum, 4, 10);

    cout << min(min3, min4) << endl;
}

int main() {
    solve();
    return 0;
}