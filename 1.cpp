#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <string_view>

#include <ranges>

using namespace std;

void part1() {
    ifstream input("input");
    string str;
    int sum = 0;
    while (getline(input, str)) {
        auto f = find_if(str.begin(), str.end(), [](const char& c) { return isdigit(c); });
        auto l = find_if(str.rbegin(), str.rend(), [](const char& c) { return isdigit(c); });
        sum += (*f - '0') * 10 + (*l - '0');
    }
    cout << sum << endl;
}

void part2() {
    ifstream input("input");
    // ofstream output("output.txt");
    string str;

    int sum = 0;

    while (getline(input, str)) {
        vector<int> v;

        // one two three four five six seven eight nine
        // because last character will be the first ,eg,
        // twone -> two, one,
        // threeight -> three, eight
        // sevenine -> seven, nine

        for (int i = 0; i < str.size();) {
            if (isdigit(str[i])) {
                v.push_back(str[i] - '0');
                ++i;
            } else {
                switch (str[i]) {
                case 'o':
                    if (string_view{&str[i], 3} == "one"sv) {
                        v.push_back(1);
                        i += 2;
                    } else {
                        ++i;
                    }
                    break;
                case 't':
                    if (string_view{&str[i], 3} == "two"sv) {
                        v.push_back(2);
                        i += 2;
                    } else if (string_view{&str[i], 5} == "three"sv) {
                        v.push_back(3);
                        i += 4;
                    } else {
                        ++i;
                    }
                    break;
                case 'f':
                    if (string_view{&str[i], 4} == "four"sv) {
                        v.push_back(4);
                        i += 4;
                    } else if (string_view{&str[i], 4} == "five"sv) {
                        v.push_back(5);
                        i += 3;
                    } else {
                        ++i;
                    }
                    break;
                case 's':
                    if (string_view{&str[i], 3} == "six"sv) {
                        v.push_back(6);
                        i += 3;
                    } else if (string_view{&str[i], 5} == "seven"sv) {
                        v.push_back(7);
                        i += 4;
                    } else {
                        ++i;
                    }

                    break;
                case 'e':
                    if (string_view{&str[i], 5} == "eight"sv) {
                        v.push_back(8);
                        i += 4;
                    } else {
                        ++i;
                    }
                    break;
                case 'n':
                    if (string_view{&str[i], 4} == "nine"sv) {
                        v.push_back(9);
                        i += 3;
                    } else {
                        ++i;
                    }
                    break;
                default:
                    ++i;
                    break;
                }
            }
        }
        // output << *v.begin() * 10 + *v.rbegin() << endl;
        sum += *v.begin() * 10 + *v.rbegin();
    }

    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}