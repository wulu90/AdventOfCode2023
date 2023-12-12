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

struct pattern {
    // string str;
    int matched_groups;
    int curr_damaged;
};

bool operator<(const pattern& lhs, const pattern& rhs) {
    if (lhs.matched_groups == rhs.matched_groups) {
        return lhs.curr_damaged < rhs.curr_damaged;
    }
    return lhs.matched_groups < rhs.matched_groups;
}

bool check(const char& c, const vector<int>& v, pattern& p) {
    if (p.matched_groups == v.size()) {
        if (c == '.') {
            // p.str += c;
            return true;
        } else {
            return false;
        }
    }

    bool res = true;
    if (c == '#') {
        if (p.curr_damaged + 1 > v[p.matched_groups]) {
            res = false;
        } else {
            // p.str += c;
            ++p.curr_damaged;
        }
    } else {
        if (p.curr_damaged > 0) {
            if (p.curr_damaged < v[p.matched_groups]) {
                res = false;
            } else {
                // p.str += c;
                p.curr_damaged = 0;
                ++p.matched_groups;
            }
        } else {
            // p.str += c;
        }
    }
    return res;
}

int64_t arrangements(const string& str, const vector<int>& v) {
    // queue<pattern> qs;
    // qs.push({});

    // for (int i = 0; i < str.size(); ++i) {
    //     size_t num = qs.size();
    //     for (int j = 0; j < num; ++j) {
    //         auto p = qs.front();

    //         if (str[i] != '?') {
    //             auto tmp = p;
    //             if (check(str[i], v, tmp) &&
    //                 (i < str.size() - 1 ||
    //                  (i == str.size() - 1 &&
    //                   (tmp.matched_groups == v.size() || (tmp.matched_groups == v.size() - 1 && tmp.curr_damaged == *v.rbegin()))))) {
    //                 qs.push(tmp);
    //             }
    //         } else {
    //             auto t1 = p;
    //             auto t2 = p;
    //             if (check('.', v, t1) &&
    //                 (i < str.size() - 1 || (i == str.size() - 1 && (t1.matched_groups == v.size() || (t1.matched_groups == v.size() - 1
    //                 &&
    //                                                                                                   t1.curr_damaged == *v.rbegin())))))
    //                                                                                                   {
    //                 qs.push(t1);
    //             }
    //             if (check('#', v, t2) &&
    //                 (i < str.size() - 1 || (i == str.size() - 1 && (t2.matched_groups == v.size() || (t2.matched_groups == v.size() - 1
    //                 &&
    //                                                                                                   t2.curr_damaged == *v.rbegin())))))
    //                                                                                                   {
    //                 qs.push(t2);
    //             }
    //         }
    //         qs.pop();
    //     }
    // }

    map<pattern, int64_t> pattern_size;
    pattern_size.insert({{}, 1});

    for (int i = 0; i < str.size(); ++i) {
        // size_t num=ps.size();
        map<pattern, int64_t> tmp_ps;

        for (auto [p, s] : pattern_size) {
            if (str[i] != '?') {
                auto tmp = p;
                if (check(str[i], v, tmp)) {
                    if (i < str.size() - 1) {
                        tmp_ps[tmp] += s;
                    }
                    if (i == str.size() - 1) {
                        if (tmp.matched_groups == v.size() || (tmp.matched_groups == v.size() - 1 && tmp.curr_damaged == *v.rbegin())) {
                            tmp_ps[tmp] += s;
                        }
                    }
                }
            } else {
                auto t1 = p;
                auto t2 = p;
                if (check('.', v, t1)) {
                    if (i < str.size() - 1) {
                        tmp_ps[t1] += s;
                    }
                    if (i == str.size() - 1) {
                        if (t1.matched_groups == v.size() || (t1.matched_groups == v.size() - 1 && t1.curr_damaged == *v.rbegin())) {
                            tmp_ps[t1] += s;
                        }
                    }
                }
                if (check('#', v, t2)) {
                    if (i < str.size() - 1) {
                        tmp_ps[t2] += s;
                    }
                    if (i == str.size() - 1) {
                        if (t2.matched_groups == v.size() || (t2.matched_groups == v.size() - 1 && t2.curr_damaged == *v.rbegin())) {
                            tmp_ps[t2] += s;
                        }
                    }
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
    int i       = 1;
    for (auto row : rows) {
        sum += arrangements(row.condition, row.damagedgroup);
        cout << i << "/" << rows.size() << " " << sum << endl;
        ++i;
    }

    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}