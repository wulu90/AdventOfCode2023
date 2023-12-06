#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void part1() {
    ifstream input("input");
    string str;

    vector<int> times;
    vector<int> dists;

    getline(input, str);

    istringstream iss;
    iss.str(str.substr(5, str.size() - 5));

    int num;
    while (iss >> num) {
        times.push_back(num);
    }

    getline(input, str);
    iss.clear();
    iss.str(str.substr(9, str.size() - 9));

    while (iss >> num) {
        dists.push_back(num);
    }

    /**
     * (T-x)*x > D
     * x^2-Tx+D<0
     * x^2-a*x+b<0
     * https://www.wolframalpha.com/input?i=x%5E2-ax%2Bb%3C0
     * (a-sqrt(a*2-4*b))/2 < x < (a+sqrt(a^2-4b))/2
     */

    num = 1;
    for (int i = 0; i < times.size(); ++i) {
        double delta = sqrt(times[i] * times[i] - 4 * dists[i]);
        double t1    = (times[i] + delta) / 2;
        double t2    = (times[i] - delta) / 2;
        num *= ceil(t1 - 1) - floor(t2 + 1) + 1;
    }

    cout << num << endl;
}

int64_t parsenum(const string& str) {
    int64_t num = 0;
    for (auto c : str) {
        if (isdigit(c)) {
            num = num * 10 + c - '0';
        }
    }
    return num;
}

void part2() {
    ifstream input("input");
    string str;

    getline(input, str);
    int64_t time = parsenum(str);
    getline(input, str);
    int64_t dist = parsenum(str);

    double delta = sqrt(time * time - 4 * dist);
    double t1    = (time + delta) / 2;
    double t2    = (time - delta) / 2;

    cout << static_cast<int64_t>(ceil(t1 - 1) - floor(t2 + 1) + 1) << endl;
}

int main() {
    part1();
    part2();
    return 0;
}