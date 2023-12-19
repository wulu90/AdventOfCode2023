#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
using namespace std;

enum class category { x, m, a, s };

category parse_category(const char c) {
    category cate;
    if (c == 'x') {
        cate = category::x;
    } else if (c == 'm') {
        cate = category::m;
    } else if (c == 'a') {
        cate = category::a;
    } else {
        cate = category::s;
    }

    return cate;
}

struct condit {
    category cate;
    int v;
    char c;

    condit() {
        c = '=';
    }

    condit(const string_view& sv) {
        cate = parse_category(sv[0]);
        c    = sv[1];
        from_chars(sv.data() + 2, sv.data() + sv.size(), v);
    }
};

function<bool(int, int)> func(char c) {
    switch (c) {
    case '>':
        return greater<int>();
    case '<':
        return less<int>();
    default:
        return [](int lhs, int rhs) { return true; };
    }
}

struct rule {
    condit cond;
    string next;

    rule(string_view sv) {
        if (sv.contains(':')) {
            auto sv_split = sv | views::split(":"sv);
            cond          = {string_view{*sv_split.begin()}};
            next          = string_view{*++sv_split.begin()};
        } else {
            next = sv;
        }
    }
};

pair<string, vector<rule>> parse_workflow(const string& str) {
    auto left_curly_braces = str.find('{');
    auto name              = str.substr(0, left_curly_braces);
    auto rulesv            = string_view{str.begin() + left_curly_braces + 1, str.end() - 1};
    vector<rule> rules;
    for (auto sv : rulesv | views::split(","sv)) {
        rules.emplace_back(string_view{sv});
    }
    return {name, rules};
}

map<category, int> parse_prop(const string_view& sv) {
    map<category, int> props;

    for (auto svp : sv | views::split(","sv)) {
        string_view svpsv{svp};
        auto cate = parse_category(svpsv[0]);
        int v;
        from_chars(&svpsv[2], svpsv.data() + svpsv.size(), v);
        props.insert({cate, v});
    }
    return props;
}

bool process(map<category, int>& prop, map<string, vector<rule>>& workflows) {
    string curr{"in"};
    bool accept = false;
    while (true) {
        for (auto& r : workflows[curr]) {
            auto lhs = prop[r.cond.cate];
            if (func(r.cond.c)(lhs, r.cond.v)) {
                curr = r.next;
                break;
            }
        }

        if (curr == "A"s) {
            accept = true;
            break;
        } else if (curr == "R"s) {
            accept = false;
            break;
        }
    }
    return accept;
}

int sum_v(const map<category, int>& prop) {
    int res = 0;
    for (auto [k, v] : prop) {
        res += v;
    }
    return res;
}

void part1() {
    ifstream input("input");
    string str;

    map<string, vector<rule>> workflows;
    while (getline(input, str) && !str.empty()) {
        workflows.insert(parse_workflow(str));
    }

    vector<map<category, int>> propvec;
    while (getline(input, str)) {
        propvec.push_back(parse_prop(string_view{str.begin() + 1, str.end() - 1}));
    }

    int sum = 0;
    for (auto& prop : propvec) {
        if (process(prop, workflows)) {
            sum += sum_v(prop);
        }
    }

    cout << sum << endl;
}

void print(map<category, pair<int, int>>& state) {
    int64_t aa = 1;
    for (auto [k, v] : state) {
        switch (k) {
        case category::x:
            cout << 'x';
            break;
        case category::m:
            cout << 'm';
            break;
        case category::a:
            cout << 'a';
            break;
        default:
            cout << 's';

            break;
        }
        aa *= v.second - v.first + 1;
        cout << " " << v.first << " " << v.second << endl;
    }

    cout << aa << endl;
}

int64_t pppp(map<string, vector<rule>>& workflows, string curr, map<category, pair<int, int>> state, int64_t sum) {
    if (curr == "A"s) {
        int64_t aa = 1;
        for (auto [k, v] : state) {
            aa *= v.second - v.first + 1;
        }
        return aa;
    }
    if (curr == "R"s) {
        return 0;
    }

    for (auto r : workflows[curr]) {
        if (r.cond.c == '>') {
            if (state[r.cond.cate].first < r.cond.v && state[r.cond.cate].second > r.cond.v) {
                auto tmp               = state;
                tmp[r.cond.cate].first = r.cond.v + 1;    // pass condition
                sum += pppp(workflows, r.next, tmp, 0);
                state[r.cond.cate].second = r.cond.v;
            } else if (state[r.cond.cate].first > r.cond.v) {
                sum += pppp(workflows, r.next, state, 0);
                break;
            }

        } else if (r.cond.c == '<') {
            if (state[r.cond.cate].first < r.cond.v && state[r.cond.cate].second > r.cond.v) {
                auto tmp                = state;
                tmp[r.cond.cate].second = r.cond.v - 1;    // pass condition
                sum += pppp(workflows, r.next, tmp, 0);
                state[r.cond.cate].first = r.cond.v;
            } else if (state[r.cond.cate].second < r.cond.v) {
                sum += pppp(workflows, r.next, state, 0);
                break;
            }

        } else {
            sum += pppp(workflows, r.next, state, 0);
            break;
        }
    }
    return sum;
}

void part2() {
    ifstream input("input");
    string str;

    map<string, vector<rule>> workflows;
    while (getline(input, str) && !str.empty()) {
        workflows.insert(parse_workflow(str));
    }

    map<category, pair<int, int>> start{
        {category::x, {1, 4000}}, {category::m, {1, 4000}}, {category::a, {1, 4000}}, {category::s, {1, 4000}}};

    auto sum = pppp(workflows, "in"s, start, 0);
    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}