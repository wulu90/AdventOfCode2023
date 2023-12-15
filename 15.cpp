#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
using namespace std;

int hash_str(const string& str) {
    int curr = 0;
    for (auto& c : str) {
        curr += static_cast<int>(c);
        curr = curr * 17 % 256;
    }
    return curr;
}

void part1() {
    ifstream input("input");
    vector<string> steps;

    for (string str; getline(input, str, ',');) {
        steps.push_back(str);
    }
    *steps.rbegin() = steps.rbegin()->substr(0, steps.rbegin()->size() - 1);    // remove last '\n'
    int sum         = 0;
    for (auto& s : steps) {
        sum += hash_str(s);
    }

    cout << sum << endl;
}

int main() {
    part1();
    return 0;
}