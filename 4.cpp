#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
using namespace std;

void parse_nums(string_view sv, set<int>& nums) {
    int num;
    istringstream iss{{sv.begin(), sv.end()}};

    while (iss >> num) {
        nums.insert(num);
    }

    // for (auto cs : sv | ranges::views::split(" "sv)) {
    //     from_chars(&*cs.begin(), &*cs.end(), num);
    //     nums.insert(num);
    // }
}

void part1() {
    ifstream input{"input"};
    string str;

    int sum = 0;

    while (getline(input, str)) {
        auto i   = str.find(":") + 2;
        auto svs = string_view{&str[i], str.size() - i} | views::split("|"sv);

        set<int> win_nums;
        parse_nums(string_view{*svs.begin()}, win_nums);

        set<int> you_nums;
        parse_nums(string_view{*++svs.begin()}, you_nums);

        int match = 0;
        for (auto n : you_nums) {
            if (win_nums.contains(n)) {
                ++match;
            }
        }

        sum += pow(2, match - 1);
    }

    cout << sum << endl;
}

void part2() {
    ifstream input{"input"};
    string str;

    vector<int> matchs;
    while (getline(input, str)) {
        auto i   = str.find(":") + 2;
        auto svs = string_view{&str[i], str.size() - i} | views::split("|"sv);

        set<int> win_nums;
        parse_nums(string_view{*svs.begin()}, win_nums);

        set<int> you_nums;
        parse_nums(string_view{*++svs.begin()}, you_nums);

        int match = 0;
        for (auto n : you_nums) {
            if (win_nums.contains(n)) {
                ++match;
            }
        }

        matchs.push_back(match);
    }

    vector<int> card_copies(matchs.size(), 1);

    for (int i = 0; i < matchs.size(); ++i) {
        for (int j = 0; j < matchs[i]; ++j) {
            card_copies[i + 1 + j] += card_copies[i];
        }
    }

    cout << accumulate(card_copies.begin(), card_copies.end(), 0) << endl;
}

int main() {
    part1();
    part2();
    return 0;
}