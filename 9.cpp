#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct history {
    vector<int64_t> values;
    size_t size;

    history(const string& str) {
        istringstream iss{str};
        int64_t n;
        while (iss >> n) {
            values.push_back(n);
        }
        size = values.size();
    }

    void process(size_t start = 0) {
        if (!all_of(values.begin() + start, values.end(), [](auto n) { return n == 0; })) {
            auto tmp = values.size();
            vector<int64_t> tv;
            adjacent_difference(values.begin() + start, values.end(), back_inserter(tv));    // why back_inserter(values) ?
            for (int i = 1; i < tv.size(); ++i) {
                values.push_back(tv[i]);
            }
            process(tmp);
        }
    }

    auto getlast() {
        process(0);

        size_t i    = size - 1;
        size_t step = size - 1;
        int64_t num = 0;
        while (i < values.size()) {
            num += values[i];
            i += step;
            --step;
        }
        return num;
    }

    auto getprev() {
        process(0);

        auto i    = 0;
        auto step = size;
        vector<size_t> inx;

        while (i < values.size()) {
            inx.push_back(i);
            i = i + step;
            --step;
        }

        int64_t tmp = 0;
        for (auto it = inx.rbegin(); it != inx.rend(); ++it) {
            tmp = values[*it] - tmp;
        }
        return tmp;
    }
};

void part1() {
    ifstream input("input");
    string str;
    istringstream iss;

    vector<history> his_vec;
    int64_t n;
    while (getline(input, str)) {
        history h(str);
        his_vec.push_back(h);
    }

    int64_t sum = 0;
    for (auto h : his_vec) {
        sum += h.getlast();
    }

    cout << sum << endl;
}

void part2() {
    ifstream input("input");
    string str;
    istringstream iss;

    vector<history> his_vec;
    int64_t n;
    while (getline(input, str)) {
        history h(str);
        his_vec.push_back(h);
    }

    int64_t sum = 0;
    for (auto h : his_vec) {
        sum += h.getprev();
    }

    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}