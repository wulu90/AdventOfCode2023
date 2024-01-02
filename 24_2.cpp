#include <array>
#include <boost/multiprecision/cpp_int.hpp>
#include <charconv>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

using namespace std;
using namespace boost::multiprecision;

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

array<array<int64_t, 3>, 3> cross(const array<int64_t, 3>& p) {
    return {{{0, -p[2], p[1]}, {p[2], 0, -p[0]}, {-p[1], p[0], 0}}};
}

array<int64_t, 3> matvecmul(const array<array<int64_t, 3>, 3>& mat, const array<int64_t, 3>& b) {
    array<int64_t, 3> c{0ll, 0ll, 0ll}; // important 
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            c[i] += mat[i][j] * b[j];
        }
    }
    return c;
}

void printmat(const array<array<int1024_t, 7>, 6>& mat) {
    for (size_t i = 0; i < 6; ++i) {
        for (size_t j = 0; j < 7; ++j) {
            cout << mat[i][j] << ' ';
        }
        cout << '\n';
    }
}

void printvec(const array<int64_t, 3>& vec) {
    for (size_t i = 0; i < 3; ++i) {
        cout << vec[i] << ' ';
    }
    cout << '\n';
}

void printmat33(const array<array<int64_t, 3>, 3>& mat) {
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            cout << mat[i][j] << ' ';
        }
        cout << '\n';
    }
}

void gauss_elimination(array<array<int1024_t, 7>, 6>& mat) {
    auto n = mat.size();
    for (size_t i = 0; i < n; ++i) {    // column 0..6,  Augmented matrix
        size_t maxrow = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (abs(mat[j][i]) > abs(mat[maxrow][i])) {
                maxrow = j;
            }
        }

        swap(mat[maxrow], mat[i]);

        for (size_t j = i + 1; j < n; ++j) {
            if (mat[j][i] == 0) {
                continue;
            }
            auto l  = lcm(mat[j][i], mat[i][i]);
            auto fi = l / mat[i][i];
            auto ff = l / mat[j][i];
            for (size_t k = i; k <= n; ++k) {
                mat[j][k] = mat[j][k] * ff - mat[i][k] * fi;
            }
        }
    }

    // printmat(mat);

    for (size_t i = n - 1; i < n; --i) {
        mat[i][n] /= mat[i][i];
        mat[i][i] = 1;
        for (size_t j = 0; j < i; ++j) {
            mat[j][n] -= mat[j][i] * mat[i][n];
            mat[j][i] = 0;
        }
    }
}

void part2() {
    ifstream input{"input"s};
    vector<hailstone> hailstonevec;
    for (string str; getline(input, str);) {
        hailstonevec.emplace_back(str);
    }

    array<int64_t, 3> dp0{hailstonevec[1].x - hailstonevec[0].x, hailstonevec[1].y - hailstonevec[0].y,
                          hailstonevec[1].z - hailstonevec[0].z};
    array<int64_t, 3> dp1{hailstonevec[2].x - hailstonevec[1].x, hailstonevec[2].y - hailstonevec[1].y,
                          hailstonevec[2].z - hailstonevec[1].z};

    array<int64_t, 3> dv0{hailstonevec[1].vx - hailstonevec[0].vx, hailstonevec[1].vy - hailstonevec[0].vy,
                          hailstonevec[1].vz - hailstonevec[0].vz};
    array<int64_t, 3> dv1{hailstonevec[2].vx - hailstonevec[1].vx, hailstonevec[2].vy - hailstonevec[1].vy,
                          hailstonevec[2].vz - hailstonevec[1].vz};

    auto dp0x = cross(dp0);
    auto dp1x = cross(dp1);

    auto dv0x = cross(dv0);
    auto dv1x = cross(dv1);

    // M=[[-dv0x,dp0x],[-dv1x,dp1x]]
    array<array<int1024_t, 7>, 6> coeffs;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            coeffs[i][j]         = -dv0x[i][j];
            coeffs[i][j + 3]     = dp0x[i][j];
            coeffs[i + 3][j]     = -dv1x[i][j];
            coeffs[i + 3][j + 3] = dp1x[i][j];
        }
    }

    auto p0x = cross({hailstonevec[0].x, hailstonevec[0].y, hailstonevec[0].z});
    auto p1x = cross({hailstonevec[1].x, hailstonevec[1].y, hailstonevec[1].z});
    auto p2x = cross({hailstonevec[2].x, hailstonevec[2].y, hailstonevec[2].z});

    auto p0xv0 = matvecmul(p0x, {hailstonevec[0].vx, hailstonevec[0].vy, hailstonevec[0].vz});
    auto p1xv1 = matvecmul(p1x, {hailstonevec[1].vx, hailstonevec[1].vy, hailstonevec[1].vz});
    auto p2xv2 = matvecmul(p2x, {hailstonevec[2].vx, hailstonevec[2].vy, hailstonevec[2].vz});

    for (size_t i = 0; i < 3; ++i) {
        coeffs[i][6]     = p1xv1[i] - p0xv0[i];
        coeffs[i + 3][6] = p2xv2[i] - p1xv1[i];
    }

    gauss_elimination(coeffs);

    // printmat(coeffs);

    cout << coeffs[0][6] + coeffs[1][6] + coeffs[2][6] << endl;
}

int main() {
    // part1();
    part2();
    return 0;
}

// https://github.com/cranil/aoc2023-rs/blob/main/src/day24.rs
// https://www.reddit.com/r/adventofcode/comments/18pnycy/comment/kepu26z/

// find_package(Boost)
// target_link_libraries(day24 Boost::headers)