#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct node {
    char direction;
    int32_t meters;
    int32_t color;

    node(const string& str) {
        direction      = str[0];
        auto lastspace = str.find(' ', 2);
        meters         = stoi(str.substr(2, lastspace - 2));
        std::size_t pos{};
        color = stoi(str.substr(lastspace + 3, 6), &pos, 16);
    }
};

void part1() {
    ifstream input("input");
    vector<node> plan;
    for (string str; getline(input, str);) {
        plan.emplace_back(str);
    }

    vector<pair<int, int>> coords{{0, 0}};

    for (auto [d, m, c] : plan) {
        auto [x, y] = *coords.rbegin();
        {
            if (d == 'R') {
                for (int i = 1; i <= m; ++i) {
                    coords.push_back({x, y + i});
                }
            } else if (d == 'D') {
                for (int i = 1; i <= m; ++i) {
                    coords.push_back({x + i, y});
                }
            } else if (d == 'L') {
                for (int i = 1; i <= m; ++i) {
                    coords.push_back({x, y - i});
                }
            } else {
                for (int i = 1; i <= m; ++i) {
                    coords.push_back({x - i, y});
                }
            }
        }
    }

    int area = 0;
    for (size_t i = 0; i < coords.size() - 1; ++i) {
        area += coords[i].first * coords[i + 1].second - coords[i].second * coords[i + 1].first;
    }

    cout << abs(area) / 2 + 1 - (coords.size() - 1) / 2 + coords.size() - 1 << endl;
}

int main() {
    part1();
    return 0;
}