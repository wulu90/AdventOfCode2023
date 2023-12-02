#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
using namespace std;

void parse(map<string, int>& cubes, const string_view& sv) {
    auto i = sv.find(' ');

    int num;

    from_chars(&sv[0], &sv[i], num);

    cubes.insert({{sv.begin() + i + 1, sv.end()}, num});
}

bool test_child(const string_view& sv) {
    map<string, int> cubes;

    for (auto ss : sv | ranges::views::split(", "sv)) {
        parse(cubes, string_view{ss});
    }

    if (cubes["red"] > 12 || cubes["green"] > 13 || cubes["blue"] > 14) {
        return false;
    }
    return true;
}

void part1() {
    ifstream input("input");
    string str;
    int id  = 1;
    int sum = 0;
    while (getline(input, str)) {
        auto start = str.find(':') + 2;

        for (auto ss : string_view{&str[start], str.size() - start} | views::split("; "sv)) {
            if (!test_child(string_view{ss})) {
                sum += id;
                break;
            }
        }

        ++id;
    }

    cout << (id - 1) * id / 2 - sum << endl;
}

void parse_part2(map<string, int>& cubes, const string_view& sv) {
    auto i = sv.find(' ');

    int num;

    from_chars(&sv[0], &sv[i], num);

    string str{sv.begin() + i + 1, sv.end()};

    if (cubes[str] < num) {
        cubes[str] = num;
    }
}

void part2() {
    ifstream input("input");
    // ofstream output("output.txt");
    string str;
    int sum = 0;

    while (getline(input, str)) {
        auto start = str.find(':') + 2;
        map<string, int> cubes;
        for (auto ss : string_view{&str[start], str.size() - start} | views::split("; "sv)) {
            for (auto ss_c : ss | views::split(", "sv)) {
                parse_part2(cubes, string_view{ss_c});
            }
        }
        // output << cubes["red"s] << " " << cubes["green"s] << " " << cubes["blue"s] << endl;
        sum += cubes["red"s] * cubes["green"s] * cubes["blue"s];
    }

    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}