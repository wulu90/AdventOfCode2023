#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> dirs{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

void backtracking(size_t endinx, size_t curr, vector<bool>& visited, size_t step, size_t& maxstep, size_t rownum, size_t colnum,
                  const vector<string>& hikingmap) {
    if (curr == endinx) {
        maxstep = max(maxstep, step);
        return;
    } else {
        auto row = curr / colnum;
        auto col = curr % colnum;
        for (auto& d : dirs) {
            auto r = row + d[0];
            auto c = col + d[1];

            if (r < rownum && c < colnum && !visited[r * colnum + c] && hikingmap[r][c] != '#') {
                auto ch = hikingmap[r][c];
                if (ch == '.') {
                    visited[r * colnum + c] = true;
                    backtracking(endinx, r * colnum + c, visited, step + 1, maxstep, rownum, colnum, hikingmap);
                    visited[r * colnum + c] = false;
                } else if (ch == '>' && c != col - 1) {
                    visited[r * colnum + c]     = true;
                    visited[r * colnum + c + 1] = true;
                    backtracking(endinx, r * colnum + c + 1, visited, step + 2, maxstep, rownum, colnum, hikingmap);
                    visited[r * colnum + c]     = false;
                    visited[r * colnum + c + 1] = false;
                } else if (ch == 'v' && r != row - 1) {
                    visited[r * colnum + c]       = true;
                    visited[(r + 1) * colnum + c] = true;
                    backtracking(endinx, (r + 1) * colnum + c, visited, step + 2, maxstep, rownum, colnum, hikingmap);
                    visited[r * colnum + c]       = false;
                    visited[(r + 1) * colnum + c] = false;
                }
            }
        }
    }
}

void part1() {
    ifstream input{"input"s};
    vector<string> hikingmap;
    for (string str; getline(input, str);) {
        hikingmap.push_back(str);
    }

    auto rownum = hikingmap.size();
    auto colnum = hikingmap.begin()->size();

    auto startinx = hikingmap.begin()->find('.');
    auto endinx   = (rownum - 1) * colnum + hikingmap.back().find('.');

    vector<bool> visited(rownum * colnum, false);
    size_t maxstep = 0;

    backtracking(endinx, startinx, visited, 0, maxstep, rownum, colnum, hikingmap);

    cout << maxstep << endl;
}


void part2() {
    ifstream input{"input"s};
    vector<string> hikingmap;
    for (string str; getline(input, str);) {
        hikingmap.push_back(str);
    }

    auto rownum = hikingmap.size();
    auto colnum = hikingmap.begin()->size();

    auto startinx = hikingmap.begin()->find('.');
    auto endinx   = (rownum - 1) * colnum + hikingmap.back().find('.');

    vector<bool> visited(rownum * colnum, false);
    
    vector<size_t> stepvec(rownum*colnum,0);

    queue<size_t> q;
    q.push(endinx);

    while(!q.empty()){
        auto qsize=q.size();

        for(size_t i=0;i<qsize;++i){
            auto curr=q.front();

            if(curr==startinx){
                break;
            }

            for(auto& d:dirs){
                auto r=curr/colnum+d[0];
                auto c=curr%colnum+d[1];

                if(r<rownum&&c<colnum&&hikingmap[r][c]!='#'){
                    
                }
            }

        }

    }

}

int main() {
    part1();
    return 0;
}