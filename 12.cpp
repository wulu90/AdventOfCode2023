#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct row {
    string condition;
    vector<int> damagedgroup;

    row(const string& str) {
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

int main() {
    part1();
    return 0;
}