#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int calc_location(const vector<string>& rocks, int r, int c) {
    int finalrow      = r;
    int cube          = -1;
    int rounded_count = 0;
    for (int i = r - 1; i >= 0; --i) {
        if (rocks[i][c] == '#') {
            cube = i;
            break;
        } else {
            if (rocks[i][c] == 'O') {
                ++rounded_count;
            }
        }
    }
    finalrow = cube + rounded_count + 1;
    return finalrow;
}

void part1() {
    ifstream input("input");
    vector<string> rocks;

    for (string str{}; getline(input, str);) {
        rocks.push_back(str);
    }

    int sum = 0;
    for (int i = 0; i < rocks.size(); ++i) {
        for (int j = 0; j < rocks.begin()->size(); ++j) {
            if (rocks[i][j] == 'O') {
                sum += rocks.size() - calc_location(rocks, i, j);
            }
        }
    }
    cout << sum << endl;
}

int main() {
    part1();
    return 0;
}