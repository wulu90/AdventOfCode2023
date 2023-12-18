#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class direction { R, D, L, U };
vector<vector<int>> dirsum{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

struct node {
    direction dire;
    int32_t meters;
    int32_t color;

    node(const string& str) {
        switch (str[0]) {
        case 'R':
            dire = direction::R;
            break;
        case 'D':
            dire = direction::D;
            break;
        case 'L':
            dire = direction::L;
            break;
        default:
            dire = direction::U;
            break;
        }
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

    int point_boundary = 0;
    for (auto [d, m, c] : plan) {
        auto [x, y] = *coords.rbegin();
        coords.push_back({x + dirsum[int(d)][0] * m, y + dirsum[int(d)][1] * m});

        point_boundary += m;
    }

    int area = 0;
    for (size_t i = 0; i < coords.size() - 1; ++i) {
        area += coords[i].first * coords[i + 1].second - coords[i].second * coords[i + 1].first;
    }

    cout << abs(area) / 2 + 1 + (point_boundary) / 2 << endl;
}

struct node2 {
    direction d;
    int64_t m;

    node2(const string& str) {
        auto lastspace = str.find(' ', 2);
        std::size_t pos{};
        m = stoi(str.substr(lastspace + 3, 5), &pos, 16);
        d = direction{str[str.size() - 2] - '0'};
    }
};

void part2() {
    ifstream input("input");
    vector<node2> plan;
    for (string str; getline(input, str);) {
        plan.emplace_back(str);
    }

    vector<pair<int64_t, int64_t>> coords{{0, 0}};

    int64_t point_boundary = 0;
    for (auto [d, m] : plan) {
        auto [x, y] = *coords.rbegin();
        coords.push_back({x + dirsum[int(d)][0] * m, y + dirsum[int(d)][1] * m});
        point_boundary += m;
    }

    int64_t area = 0;
    for (size_t i = 0; i < coords.size() - 1; ++i) {
        area += coords[i].first * coords[i + 1].second - coords[i].second * coords[i + 1].first;
    }

    cout << abs(area) / 2 + 1 + (point_boundary) / 2 << endl;
}

int main() {
    part1();
    part2();
    return 0;
}