#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

enum class direction { up, left, down, right };

void dfs(const vector<string>& layout, size_t row, size_t col, direction dire, vector<vector<set<direction>>>& grid_dirs) {
    size_t rowsize = layout.size();
    size_t colsize = layout[0].size();

    if (row < 0 || row >= rowsize || col < 0 || col >= colsize) {
        return;
    }

    if (grid_dirs[row][col].contains(dire)) {
        return;
    }
    grid_dirs[row][col].insert(dire);
    char c = layout[row][col];

    if (dire == direction::up) {
        if (c == '.' || c == '|') {
            dfs(layout, row - 1, col, direction::up, grid_dirs);
        } else if (c == '-') {
            dfs(layout, row, col - 1, direction::left, grid_dirs);
            dfs(layout, row, col + 1, direction::right, grid_dirs);
        } else if (c == '/') {
            dfs(layout, row, col + 1, direction::right, grid_dirs);
        } else {    // '\\'
            dfs(layout, row, col - 1, direction::left, grid_dirs);
        }
    } else if (dire == direction::left) {
        if (c == '.' || c == '-') {
            dfs(layout, row, col - 1, direction::left, grid_dirs);
        } else if (c == '|') {
            dfs(layout, row - 1, col, direction::up, grid_dirs);
            dfs(layout, row + 1, col, direction::down, grid_dirs);
        } else if (c == '/') {
            dfs(layout, row + 1, col, direction::down, grid_dirs);
        } else {
            dfs(layout, row - 1, col, direction::up, grid_dirs);
        }
    } else if (dire == direction::down) {
        if (c == '.' || c == '|') {
            dfs(layout, row + 1, col, direction::down, grid_dirs);
        } else if (c == '-') {
            dfs(layout, row, col - 1, direction::left, grid_dirs);
            dfs(layout, row, col + 1, direction::right, grid_dirs);
        } else if (c == '/') {
            dfs(layout, row, col - 1, direction::left, grid_dirs);
        } else {
            dfs(layout, row, col + 1, direction::right, grid_dirs);
        }
    } else {    // direction::right
        if (c == '.' || c == '-') {
            dfs(layout, row, col + 1, direction::right, grid_dirs);
        } else if (c == '|') {
            dfs(layout, row - 1, col, direction::up, grid_dirs);
            dfs(layout, row + 1, col, direction::down, grid_dirs);
        } else if (c == '/') {
            dfs(layout, row - 1, col, direction::up, grid_dirs);
        } else {
            dfs(layout, row + 1, col, direction::down, grid_dirs);
        }
    }
}

void part1() {
    ifstream input("input");
    vector<string> layout;
    for (string str; getline(input, str);) {
        layout.push_back(str);
    }

    vector<vector<set<direction>>> grid_dirs(layout.size(), vector<set<direction>>(layout[0].size()));

    dfs(layout, 0, 0, direction::right, grid_dirs);

    size_t energized_count = 0;
    for (auto& sv : grid_dirs) {
        for (auto& s : sv) {
            if (!s.empty()) {
                ++energized_count;
            }
        }
    }
    cout << energized_count << endl;
}

int main() {
    part1();
    return 0;
}