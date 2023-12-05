#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int64_t conv(int64_t sour, const map<int64_t, pair<int64_t, int64_t>>& cmap) {
    int64_t res = 0;
    if (sour < cmap.begin()->first || sour >= cmap.rbegin()->first + cmap.rbegin()->second.second) {
        res = sour;
    } else {
        for (auto it = cmap.begin(); it != cmap.end(); ++it) {
            if (sour >= it->first && sour < it->first + it->second.second) {
                res = sour - it->first + it->second.first;
                break;
            }
            res = sour;    // if conver map not contiguous; this is the key code
        }
    }

    return res;
}

void part1() {
    ifstream input("input");
    string str;

    getline(input, str);

    istringstream iss{str.substr(7, str.size() - 7)};
    int64_t num;

    vector<int64_t> seeds;
    while (iss >> num) {
        seeds.push_back(num);
    }
    iss.clear();
    getline(input, str);    // blank line;

    vector<string> imaps;

    while (getline(input, str)) {
        if (!str.contains('-')) {
            imaps.push_back(str);
        }
    }

    int64_t dest, sour, len;

    vector<map<int64_t, pair<int64_t, int64_t>>> convertmaps;
    for (auto sv : imaps | views::split(string{})) {
        map<int64_t, pair<int64_t, int64_t>> cmap;
        for (auto& s : sv) {
            iss.str(s);
            iss >> dest >> sour >> len;
            iss.clear();
            cmap.insert({sour, {dest, len}});
        }
        convertmaps.push_back(cmap);
    }

    vector<int64_t> locations;
    for (auto seed : seeds) {
        sour = seed;
        for (auto& m : convertmaps) {
            sour = conv(sour, m);
        }
        locations.push_back(sour);
    }

    cout << *min_element(locations.begin(), locations.end()) << endl;
}

void part2() {
    ifstream input("input");
    string str;

    getline(input, str);

    istringstream iss{str.substr(7, str.size() - 7)};
    int64_t num;

    vector<int64_t> seeds;
    while (iss >> num) {
        seeds.push_back(num);
    }
    iss.clear();
    getline(input, str);    // blank line;

    vector<string> imaps;

    while (getline(input, str)) {
        if (!str.contains('-')) {
            imaps.push_back(str);
        }
    }

    int64_t dest, sour, len;

    vector<map<int64_t, pair<int64_t, int64_t>>> convertmaps;
    for (auto sv : imaps | views::split(string{})) {
        map<int64_t, pair<int64_t, int64_t>> cmap;
        for (auto& s : sv) {
            iss.str(s);
            iss >> dest >> sour >> len;
            iss.clear();
            cmap.insert({sour, {dest, len}});
        }
        convertmaps.push_back(cmap);
    }

    // todo: need clean overlap seeds
    int64_t minloca = numeric_limits<int64_t>::max();
    for (int i = 0; i < seeds.size(); ++i) {
        for (int64_t j = seeds[i]; j < seeds[i] + seeds[i + 1]; ++j) {
            sour = j;
            for (auto& m : convertmaps) {
                sour = conv(sour, m);
            }
            minloca = min(minloca, sour);
        }
        ++i;
    }
    cout << minloca << endl;
}

int main() {
    part1();
    part2();
    return 0;
}