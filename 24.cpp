#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

static int64_t maxv = 400000000000000ll;
static int64_t minv = 200000000000000ll;

struct hailstone {
    int64_t x;
    int64_t y;
    int64_t z;
    int64_t vx;
    int64_t vy;
    int64_t vz;

    // y=ax+b;
    double a;    // slope
    double b;

    hailstone(const string& str) {
        auto str_sp = str | views::split(" @ "sv);

        auto location_sv = string_view{*str_sp.begin()} | views::split(", "sv);

        auto tmp = location_sv.begin();
        from_chars((*tmp).data(), (*tmp).data() + (*tmp).size(), x);
        ++tmp;
        from_chars((*tmp).data(), (*tmp).data() + (*tmp).size(), y);
        ++tmp;
        from_chars((*tmp).data(), (*tmp).data() + (*tmp).size(), z);

        auto velocity_sv = string_view{*++str_sp.begin()} | views::split(", "sv);
        tmp              = velocity_sv.begin();
        from_chars((*tmp).data(), (*tmp).data() + (*tmp).size(), vx);
        ++tmp;
        from_chars((*tmp).data(), (*tmp).data() + (*tmp).size(), vy);
        ++tmp;
        from_chars((*tmp).data(), (*tmp).data() + (*tmp).size(), vz);

        a = vx == 0 ? numeric_limits<double>::max() : static_cast<double>(vy) / static_cast<double>(vx);
        b = y - a * x;
    }
};

// input data no coinindent line
bool intersection_xy(const hailstone& h1, const hailstone& h2) {
    if (h1.a != h2.a) {
        auto px = (h2.b - h1.b) / (h1.a - h2.a);
        auto py = h1.a * px + h1.b;
        if (px > minv && px < maxv && py > minv && py < maxv) {
            auto xt1 = (px - h1.x) / h1.vx;
            auto yt1 = (py - h1.y) / h1.vy;
            auto xt2 = (px - h2.x) / h2.vx;
            auto yt2 = (py - h2.y) / h2.vy;

            if (xt1 > 0 && yt1 > 0 && xt2 > 0 && yt2 > 0) {
                return true;
            }
        }
    }
    return false;
}

void part1() {
    ifstream input{"input"s};
    vector<hailstone> hailstonevec;
    for (string str; getline(input, str);) {
        hailstonevec.emplace_back(str);
    }

    int num = 0;

    for (size_t i = 0; i < hailstonevec.size() - 1; ++i) {
        for (size_t j = i + 1; j < hailstonevec.size(); ++j) {
            if (intersection_xy(hailstonevec[i], hailstonevec[j])) {
                ++num;
            }
        }
    }

    cout << num << endl;
}

int main() {
    part1();
    return 0;
}