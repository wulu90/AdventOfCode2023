#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <z3++.h>

using namespace std;
using namespace z3;

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

void part2() {
    ifstream input{"input"s};
    vector<hailstone> hailstonevec;
    for (string str; getline(input, str);) {
        hailstonevec.emplace_back(str);
    }

    z3::context c;
    // set_param("verbose", 10);
    z3::expr_vector t(c);

    for (size_t i = 0; i < hailstonevec.size(); ++i) {
        string name = "t"s + to_string(i);
        t.push_back(c.int_const(name.c_str()));
    }

    z3::expr x  = c.int_const("x");
    z3::expr y  = c.int_const("y");
    z3::expr z  = c.int_const("z");
    z3::expr vx = c.int_const("vx");
    z3::expr vy = c.int_const("vy");
    z3::expr vz = c.int_const("vz");

    z3::solver solver(c);

    for (size_t i = 0; i < hailstonevec.size(); ++i) {
        solver.add(x + vx * t[i] - c.int_val(hailstonevec[i].x) - c.int_val(hailstonevec[i].vx) * t[i] == 0);
        solver.add(y + vy * t[i] - c.int_val(hailstonevec[i].y) - c.int_val(hailstonevec[i].vy) * t[i] == 0);
        solver.add(z + vz * t[i] - c.int_val(hailstonevec[i].z) - c.int_val(hailstonevec[i].vz) * t[i] == 0);
    }

    cout << solver.check() << endl;
    auto m = solver.get_model();
    cout << m.eval(x) << endl;
    cout << m.eval(y) << endl;
    cout << m.eval(z) << endl;
    cout << m.eval(vx) << endl;
    cout << m.eval(vy) << endl;
    cout << m.eval(vz) << endl;
    cout << m.eval(x + y + z) << endl;
}

int main() {
    part2();
    return 0;
}
