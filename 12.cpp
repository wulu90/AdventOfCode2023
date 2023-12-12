#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct row {
    string condition;
    vector<int> damagedgroup;

    row(const string& str, bool unfold = false) {
        auto space_index = str.find(' ');
        condition        = str.substr(0, space_index);

        istringstream iss(str.substr(space_index, str.size() - space_index));
        int tmp;
        while (iss >> tmp) {
            damagedgroup.push_back(tmp);
            if (iss.peek() == ',') {
                iss.ignore();
            }
        }

        if (unfold) {
            auto tmpstr = condition;
            auto tmpvec = damagedgroup;
            for (int i = 0; i < 4; ++i) {
                tmpstr.append(1, '?');
                tmpstr.append(condition);
                tmpvec.append_range(damagedgroup);
            }
            condition    = tmpstr;
            damagedgroup = tmpvec;
        }
    }
};

vector<string> possible_start(string str, int rowlen, int damage_num, bool islast) {
    vector<string> res;

    int remain = islast ? rowlen - str.size() - damage_num : rowlen - str.size() - damage_num - 1;

    for (int i = 0; i <= remain; ++i) {
        string s = str;
        s.append(i, '.');
        res.push_back(s);
    }

    for (auto& s : res) {
        s.append(damage_num, '#');
        if (!islast) {
            s.append(1, '.');
        }
        if (islast && s.size() < rowlen) {
            s.append(rowlen - s.size(), '.');
        }
    }

    return res;
}

vector<string> all_arrangement(int rowlen, const vector<int>& groups) {
    vector<string> aa = possible_start({}, rowlen, groups[0], false);

    for (int i = 1; i < groups.size(); ++i) {
        vector<string> tmp;
        for (auto s : aa) {
            auto v = possible_start(s, rowlen, groups[i], i == groups.size() - 1);
            tmp.append_range(v);
        }
        aa = tmp;
    }

    // for (auto a : aa) {
    //     cout << a << endl;
    // }
    // cout << endl;
    return aa;
}

int check(const vector<string>& aa, string str) {
    int count = aa.size();
    for (auto& a : aa) {
        for (int i = 0; i < a.size(); ++i) {
            if (str[i] != '?' && str[i] != a[i]) {
                --count;
                break;
            }
        }
    }
    return count;
}

void part1() {
    ifstream input("input");
    string str;
    vector<row> rows;
    while (getline(input, str)) {
        rows.emplace_back(str);
    }

    int sum = 0;
    for (auto row : rows) {
        sum += check(all_arrangement(row.condition.size(), row.damagedgroup), row.condition);
    }

    cout << sum << endl;
}

bool check_nextchar(char c, const vector<int>& v, pair<int, int>& p) {
    auto& [matched_group, curr_damaged] = p;
    if (matched_group == v.size()) {
        if (c == '.') {
            return true;
        } else {
            return false;
        }
    }

    bool res = true;

    if (c == '#') {
        if (curr_damaged + 1 > v[matched_group]) {
            res = false;
        } else {
            ++curr_damaged;
        }
    } else {
        if (curr_damaged > 0) {
            if (curr_damaged < v[matched_group]) {
                res = false;
            } else {
                curr_damaged = 0;
                ++matched_group;
            }
        }
    }
    return res;
}

bool check_pattern(int inx, int strlen, const vector<int>& v, const pair<int, int>& p) {
    bool res = false;
    if (inx < strlen - 1) {
        res = true;
    } else {
        auto [matched_group, curr_damaged] = p;
        if (matched_group == v.size() || (matched_group == v.size() - 1 && curr_damaged == *v.rbegin())) {
            res = true;
        }
    }
    return res;
}

int64_t arr(const string& str, const vector<int>& v) {
    map<pair<int, int>, int64_t> pattern_size;    // matched_group,curr_damaged,count;
    pattern_size.insert({{}, 1});

    for (int i = 0; i < str.size(); ++i) {
        map<pair<int, int>, int64_t> tmp_ps;

        for (auto [p, s] : pattern_size) {
            vector<char> cv;
            if (str[i] != '?') {
                cv.push_back(str[i]);
            } else {
                cv.push_back('.');
                cv.push_back('#');
            }

            for (auto c : cv) {
                auto tmp = p;

                if (check_nextchar(c, v, tmp) && check_pattern(i, str.size(), v, tmp)) {
                    tmp_ps[tmp] += s;
                }
            }
        }
        pattern_size = tmp_ps;
    }
    int64_t sum = 0;
    for (auto [p, s] : pattern_size) {
        sum += s;
    }
    return sum;
}

void part2() {
    ifstream input("input");
    string str;
    vector<row> rows;
    while (getline(input, str)) {
        rows.emplace_back(str, true);
    }

    int64_t sum = 0;
    for (auto row : rows) {
        sum += arr(row.condition, row.damagedgroup);
    }

    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}