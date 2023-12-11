#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

int expand_count(const vector<int> blanks, int s, int e) {
    int l     = min(s, e);
    int r     = max(s, e);
    int count = 0;

    for (int i = 0; i < blanks.size(); ++i) {
        if (blanks[i] > l && blanks[i] < r) {
            ++count;
        }
    }
    return count;
}

void part1() {
    ifstream input("input");
    string str;
    vector<string> galaxymap;
    while (getline(input, str)) {
        galaxymap.push_back(str);
    }

    vector<pair<int, int>> galaxys;
    for (int i = 0; i < galaxymap.size(); ++i) {
        for (int j = 0; j < galaxymap[0].size(); ++j) {
            if (galaxymap[i][j] == '#') {
                galaxys.push_back({i, j});
            }
        }
    }

    vector<int> blankrows;
    vector<int> blankcols;

    for (int i = 0; i < galaxymap.size(); ++i) {
        if (!galaxymap[i].contains('#')) {
            blankrows.push_back(i);
        }
    }

    for (int i = 0; i < galaxymap[0].size(); ++i) {
        bool hasgalaxy = false;
        for (int j = 0; j < galaxymap.size(); ++j) {
            if (galaxymap[j][i] == '#') {
                hasgalaxy = true;
                break;
            }
        }
        if (!hasgalaxy) {
            blankcols.push_back(i);
        }
    }

    int sum = 0;
    for (int i = 0; i < galaxys.size() - 1; ++i) {
        for (int j = i + 1; j < galaxys.size(); ++j) {
            sum += abs(galaxys[i].first - galaxys[j].first) + abs(galaxys[i].second - galaxys[j].second) +
                   expand_count(blankrows, galaxys[i].first, galaxys[j].first) +
                   expand_count(blankcols, galaxys[i].second, galaxys[j].second);
        }
    }
    cout << sum << endl;
}

void part2() {
    ifstream input("input");
    string str;
    vector<string> galaxymap;
    while (getline(input, str)) {
        galaxymap.push_back(str);
    }

    vector<pair<int, int>> galaxys;
    for (int i = 0; i < galaxymap.size(); ++i) {
        for (int j = 0; j < galaxymap[0].size(); ++j) {
            if (galaxymap[i][j] == '#') {
                galaxys.push_back({i, j});
            }
        }
    }

    vector<int> blankrows;
    vector<int> blankcols;

    for (int i = 0; i < galaxymap.size(); ++i) {
        if (!galaxymap[i].contains('#')) {
            blankrows.push_back(i);
        }
    }

    for (int i = 0; i < galaxymap[0].size(); ++i) {
        bool hasgalaxy = false;
        for (int j = 0; j < galaxymap.size(); ++j) {
            if (galaxymap[j][i] == '#') {
                hasgalaxy = true;
                break;
            }
        }
        if (!hasgalaxy) {
            blankcols.push_back(i);
        }
    }

    int64_t sum = 0;
    for (int i = 0; i < galaxys.size() - 1; ++i) {
        for (int j = i + 1; j < galaxys.size(); ++j) {
            sum += abs(galaxys[i].first - galaxys[j].first) + abs(galaxys[i].second - galaxys[j].second) +
                   expand_count(blankrows, galaxys[i].first, galaxys[j].first) * 999999LL +
                   expand_count(blankcols, galaxys[i].second, galaxys[j].second) * 999999LL;
        }
    }
    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}