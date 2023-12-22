#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct endpo {
    int x;
    int y;
    int z;

    endpo() = default;

    endpo(const string_view& sv) {
        auto sv_split = sv | views::split(","sv);
        auto tmpsv    = sv_split.begin();

        from_chars((*tmpsv).data(), (*tmpsv).data() + (*tmpsv).size(), x);
        ++tmpsv;
        from_chars((*tmpsv).data(), (*tmpsv).data() + (*tmpsv).size(), y);
        ++tmpsv;
        from_chars((*tmpsv).data(), (*tmpsv).data() + (*tmpsv).size(), z);
    }
};

struct brick {
    endpo e1;
    endpo e2;

    brick(const string& str) {
        auto str_split = str | views::split("~"sv);
        e1             = endpo{string_view{*str_split.begin()}};
        e2             = endpo{string_view{*++str_split.begin()}};
    }
};

bool operator<(const brick& b1, const brick& b2) {
    return tie(b1.e1.z, b1.e1.x, b1.e1.y, b1.e2.z, b1.e2.x, b1.e2.y) < tie(b2.e1.z, b2.e1.x, b2.e1.y, b2.e2.z, b2.e2.x, b2.e2.y);
}

bool interact_xy(const brick& b1, const brick& b2) {
    return !(b1.e1.x > b2.e2.x || b1.e2.x < b2.e1.x) && !(b1.e1.y > b2.e2.y || b1.e2.y < b2.e1.y);
}

void part1() {
    ifstream input{"input"};
    vector<brick> brickvec;
    for (string str; getline(input, str);) {
        brickvec.emplace_back(str);
    }

    int minx = numeric_limits<int>::max();
    int maxx = numeric_limits<int>::min();
    int miny = minx;
    int maxy = maxx;

    for (auto [e1, e2] : brickvec) {
        minx = min(e1.x, minx);
        maxx = max(e2.x, maxx);
        miny = min(e1.y, miny);
        maxy = max(e2.y, maxy);
    }
    // because minx=miny=0
    vector<vector<int>> gridtop(maxx + 1, vector<int>(maxy + 1));

    sort(brickvec.begin(), brickvec.end());

    map<int, vector<int>> topzmapinx;
    map<int, vector<int>> botzmapint;
    for (int binx = 0; binx < brickvec.size(); ++binx) {
        auto& [e1, e2] = brickvec[binx];
        if (e1.z == 1) {
            for (int i = e1.x; i <= e2.x; ++i) {
                for (int j = e1.y; j <= e2.y; ++j) {
                    gridtop[i][j] = e2.z;
                }
            }
        } else {
            auto topz = numeric_limits<int>::min();
            for (int i = e1.x; i <= e2.x; ++i) {
                for (int j = e1.y; j <= e2.y; ++j) {
                    topz = max(topz, gridtop[i][j]);
                }
            }

            // brick fall
            if (e1.z > topz + 1) {
                auto delta = e1.z - (topz + 1);
                e1.z       = topz + 1;
                e2.z       = e2.z - delta;
            }
            // update gridtop
            for (int i = e1.x; i <= e2.x; ++i) {
                for (int j = e1.y; j <= e2.y; ++j) {
                    gridtop[i][j] = e2.z;
                }
            }
        }

        topzmapinx[e2.z].push_back(binx);
        botzmapint[e1.z].push_back(binx);
    }

    map<int, vector<int>> supportby;
    map<int, vector<int>> support;

    for (int binx = 0; binx < brickvec.size(); ++binx) {
        auto& [e1, e2] = brickvec[binx];
        if (e1.z != 1) {
            for (auto i : topzmapinx[e1.z - 1]) {
                if (interact_xy(brickvec[binx], brickvec[i])) {
                    supportby[binx].push_back(i);
                }
            }
        }

        for (auto i : botzmapint[e2.z + 1]) {
            if (interact_xy(brickvec[binx], brickvec[i])) {
                support[binx].push_back(i);
            }
        }
    }

    int count = 0;
    for (int binx = 0; binx < brickvec.size(); ++binx) {
        if (!support.contains(binx)) {
            ++count;
        } else {
            bool canbreak = true;
            for (auto i : support[binx]) {
                if (supportby[i].size() == 1) {
                    canbreak = false;
                    break;
                }
            }
            if (canbreak) {
                ++count;
            }
        }
    }

    cout << count << endl;
}

int main() {
    part1();
    return 0;
}