#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

pair<int, int> getnext(const vector<string>& surf, int i, int j, int p_i, int p_j) {
    switch (surf[i][j]) {
    case '|':
        return p_i < i ? make_pair(i + 1, j) : make_pair(i - 1, j);
    case '-':
        return p_j < j ? make_pair(i, j + 1) : make_pair(i, j - 1);
    case 'L':
        return p_j == j ? make_pair(i, j + 1) : make_pair(i - 1, j);
    case 'J':
        return p_j == j ? make_pair(i, j - 1) : make_pair(i - 1, j);
    case '7':
        return p_i == i ? make_pair(i + 1, j) : make_pair(i, j - 1);
    default:    // 'F'
        return p_i == i ? make_pair(i + 1, j) : make_pair(i, j + 1);
    }
}

tuple<int, int, int, int> getstartdirs(char s, int i, int j) {
    switch (s) {
    case '|':
        return make_tuple(i + 1, j, i - 1, j);
    case '-':
        return make_tuple(i, j - 1, i, j + 1);
    case 'L':
        return make_tuple(i - 1, j, i, j + 1);
    case 'J':
        return make_tuple(i - 1, j, i, j - 1);
    case '7':
        return make_tuple(i, j - 1, i + 1, j);
    default:    // 'F'
        return make_tuple(i, j + 1, i + 1, j);
    }
}

char findstarttype(const vector<string>& surf, int i, int j) {
    char up    = i == 0 ? '.' : surf[i - 1][j];
    char left  = j == 0 ? '.' : surf[i][j - 1];
    char down  = i == surf.size() - 1 ? '.' : surf[i + 1][j];
    char right = j == surf[0].size() - 1 ? '.' : surf[i][j + 1];

    if ((up == '|' || up == '7' || up == 'F') && (down == '|' || down == 'L' || down == 'J')) {
        return '|';
    }
    if ((left == '-' || left == 'L' || left == 'F') && (right == '-' || right == 'J' || right == '7')) {
        return '-';
    }
    if ((up == '|' || up == '7' || up == 'F') && (right == '-' || right == 'J' || right == '7')) {
        return 'L';
    }
    if ((up == '|' || up == '7' || up == 'F') && (left == '-' || left == 'L' || left == 'F')) {
        return 'J';
    }
    if ((left == '-' || left == 'L' || left == 'F') && (down == '|' || down == 'L' || down == 'J')) {
        return '7';
    }
    return 'F';
}

void part1() {
    ifstream input("input");
    vector<string> surface;
    string str;
    while (getline(input, str)) {
        surface.push_back(str);
    }

    // find start position
    int i = 0;
    int j = 0;

    while ((j = surface[i].find('S')) == string::npos) {
        ++i;
    }

    char stype = findstarttype(surface, i, j);

    auto startdirs = getstartdirs(stype, i, j);
    pair<int, int> curr_1{get<0>(startdirs), get<1>(startdirs)};
    pair<int, int> curr_2{get<2>(startdirs), get<3>(startdirs)};

    pair<int, int> prev_1{i, j};
    pair<int, int> prev_2{i, j};

    int step = 1;

    while (curr_1 != curr_2) {
        auto tmp1 = curr_1;
        auto tmp2 = curr_2;
        curr_1    = getnext(surface, curr_1.first, curr_1.second, prev_1.first, prev_1.second);
        curr_2    = getnext(surface, curr_2.first, curr_2.second, prev_2.first, prev_2.second);
        prev_1    = tmp1;
        prev_2    = tmp2;
        ++step;
    }

    cout << step << endl;
}

void part2() {
    ifstream input("input");
    vector<string> surface;
    string str;
    while (getline(input, str)) {
        surface.push_back(str);
    }

    // find start position
    int i = 0;
    int j = 0;

    while ((j = surface[i].find('S')) == string::npos) {
        ++i;
    }

    char stype = findstarttype(surface, i, j);

    auto startdirs = getstartdirs(stype, i, j);
    pair<int, int> curr_1{get<0>(startdirs), get<1>(startdirs)};
    pair<int, int> curr_2{get<2>(startdirs), get<3>(startdirs)};

    pair<int, int> prev_1{i, j};
    pair<int, int> prev_2{i, j};

    int step = 1;

    vector<pair<int, int>> point_vec1, point_vec2;

    point_vec1.push_back(prev_1);
    point_vec1.push_back(curr_1);

    point_vec2.push_back(prev_2);
    point_vec2.push_back(curr_2);

    while (curr_1 != curr_2) {
        auto tmp1 = curr_1;
        auto tmp2 = curr_2;
        curr_1    = getnext(surface, curr_1.first, curr_1.second, prev_1.first, prev_1.second);
        curr_2    = getnext(surface, curr_2.first, curr_2.second, prev_2.first, prev_2.second);
        prev_1    = tmp1;
        prev_2    = tmp2;
        point_vec1.push_back(curr_1);
        point_vec2.push_back(curr_2);
        ++step;
    }

    vector<pair<int, int>> pv;
    copy(point_vec1.begin(), point_vec1.end(), back_inserter(pv));
    copy(point_vec2.rbegin() + 1, point_vec2.rend(), back_inserter(pv));

    int area = 0;

    for (int i = 0; i < pv.size() - 1; ++i) {
        area += pv[i].first * pv[i + 1].second - pv[i].second * pv[i + 1].first;
    }
    // CSES 2193 Geometry/Polygon_Lattice_Points
    // https://en.wikipedia.org/wiki/Pick's_theorem
    cout << abs(area) / 2 + 1 - (pv.size() - 1) / 2 << endl;
}

int main() {
    // key point: there is only one loop!
    part1();
    part2();
    return 0;
}