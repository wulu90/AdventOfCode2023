#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
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

void part2() {
    ifstream input("input");
    vector<string> steps;

    for (string str; getline(input, str, ',');) {
        steps.push_back(str);
    }
    *steps.rbegin() = steps.rbegin()->substr(0, steps.rbegin()->size() - 1);    // remove last '\n'

    vector<vector<string>> boxes(256);    // lable;
    vector<map<string, size_t>> label_index_maps(256);
    map<string, int> label_focal_length;
    for (auto& s : steps) {
        bool dash        = s.contains('-');
        size_t sp        = dash ? s.find('-') : s.find('=');
        string label     = s.substr(0, sp);
        int focal_lenght = dash ? 0 : stoi(s.substr(sp + 1, s.size() - sp - 1));

        label_focal_length[label] = focal_lenght;

        int boxindex = hash_str(label);
        if (dash) {
            if (label_index_maps[boxindex].contains(label)) {
                int slotindex = label_index_maps[boxindex][label];
                boxes[boxindex].erase(boxes[boxindex].begin() + slotindex);
                label_index_maps[boxindex].erase(label);
                for (auto it = boxes[boxindex].begin() + slotindex; it != boxes[boxindex].end(); ++it) {
                    --label_index_maps[boxindex][*it];
                }
            }
        } else {
            if (!label_index_maps[boxindex].contains(label)) {
                boxes[boxindex].push_back(label);
                label_index_maps[boxindex][label] = boxes[boxindex].size() - 1;
            }
        }
    }
    int64_t sum  = 0;
    int boxindex = 1;
    for (auto& label_vec : boxes) {
        int slot = 1;
        for (auto& l : label_vec) {
            sum += boxindex * slot * label_focal_length[l];
            ++slot;
        }

        ++boxindex;
    }

    cout << sum << endl;
}

int main() {
    part1();
    part2();
    return 0;
}