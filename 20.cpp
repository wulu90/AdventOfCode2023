#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <vector>

using namespace std;

enum class pulses { low, high };
enum class MType { flip_flop, conjunct, broadcaster };

struct msg {
    string sender;
    string receiver;
    pulses pul;
};

struct module {
    string name;
    MType mtype;
    vector<string> conn_str;

    module(string&& str, vector<string>&& strvec, MType mt) :name{str}, conn_str{strvec}, mtype{mt} {}

    virtual void handle_msg(const msg& m, queue<msg>& msg_queue) = 0;
    virtual ~module()                                            = default;
};

struct flip_flop : public module {
    bool is_on;

    flip_flop(string&& str, vector<string>&& strvec) : module{move(str), move(strvec), MType::flip_flop}, is_on{false} {}

    void handle_msg(const msg& m, queue<msg>& msg_queue) override {
        if (m.pul == pulses::low) {
            is_on         = !is_on;
            auto send_pul = is_on ? pulses::high : pulses::low;
            for (auto& s : conn_str) {
                msg_queue.push({name, s, send_pul});
            }
        }
    }
};

struct conjunct : public module {
    map<string, pulses> rems;

    conjunct(string&& str, vector<string>&& strvec) : module{move(str), move(strvec), MType::conjunct} {}

    bool all_high() {
        return all_of(rems.begin(), rems.end(), [](pair<string, pulses> rem) { return rem.second == pulses::high; });
    }

    void add_input(const string& str) {
        rems.insert({str, pulses::low});
    }

    void update_input(const string& str, const pulses& pul) {
        rems[str] = pul;
    }

    void handle_msg(const msg& m, queue<msg>& msg_queue) override {
        auto send_pul = all_high() ? pulses::low : pulses::high;
        for (auto& s : conn_str) {
            msg_queue.push({name, s, send_pul});
        }
    }
};

struct broadcaster : public module {
    broadcaster(string&& str, vector<string>&& strvec) : module{move(str), move(strvec), MType::broadcaster} {}

    void handle_msg(const msg& m, queue<msg>& msg_queue) override {
        for (auto& s : conn_str) {
            msg_queue.push({name, s, pulses::low});
        }
    }
};

unique_ptr<module> parseline(const string& str) {
    auto sv = str | views::split(" -> "sv);
    string_view name_sv{*sv.begin()};

    string name = {name_sv.begin() + 1, name_sv.end()};
    if (name_sv[0] == 'b') {
        name = 'b' + name;
    }

    string_view conn_sv{*++sv.begin()};
    vector<string> strvec;
    for (auto s : conn_sv | views::split(", "sv)) {
        strvec.emplace_back(string_view{s});
    }

    unique_ptr<module> m;
    if (name_sv[0] == '%') {
        m = make_unique<flip_flop>(move(name), move(strvec));
    } else if (name_sv[0] == '&') {
        m = make_unique<conjunct>(move(name), move(strvec));
    } else {
        m = make_unique<broadcaster>(move(name), move(strvec));
    }
    return m;
}

void part1() {
    ifstream input("input");
    map<string, unique_ptr<module>> modulemap;
    for (string str; getline(input, str);) {
        auto m = parseline(str);
        modulemap.insert({m->name, move(m)});
    }

    for (auto& [str, m] : modulemap) {
        for (auto& s : m->conn_str) {
            if (modulemap.contains(s) && modulemap[s]->mtype == MType::conjunct) {
                dynamic_cast<conjunct&>(*modulemap[s]).add_input(str);

                // dynamic_cast<conjunct*>(modulemap[s].get())->add_input(str);
            }
        }
    }

    queue<msg> msg_queue;
    int64_t lowcount  = 0;
    int64_t highcount = 0;
    for (int i = 1; i <= 1000; ++i) {
        ++lowcount;    // button -> broadcaster;

        dynamic_cast<broadcaster&>(*modulemap["broadcaster"s]).handle_msg({}, msg_queue);

        while (!msg_queue.empty()) {
            auto& m = msg_queue.front();

            if (m.pul == pulses::high) {
                ++highcount;
            } else {
                ++lowcount;
            }
            if (!modulemap.contains(m.receiver)) {
                msg_queue.pop();
                continue;
            }

            if (modulemap[m.receiver]->mtype == MType::conjunct) {
                dynamic_cast<conjunct&>(*modulemap[m.receiver]).update_input(m.sender, m.pul);
            }

            modulemap[m.receiver]->handle_msg(m, msg_queue);

            msg_queue.pop();
        }
    }

    cout << highcount * lowcount << endl;
}

void part2() {
    ifstream input("input");
    map<string, unique_ptr<module>> modulemap;
    for (string str; getline(input, str);) {
        auto m = parseline(str);
        modulemap.insert({m->name, move(m)});
    }

    for (auto& [str, m] : modulemap) {
        for (auto& s : m->conn_str) {
            if (modulemap.contains(s) && modulemap[s]->mtype == MType::conjunct) {
                dynamic_cast<conjunct&>(*modulemap[s]).add_input(str);
            }
        }
    }

    vector<string> rx_grandfathers;

    for (auto& [str, m] : modulemap) {
        if (count(m->conn_str.begin(), m->conn_str.end(), "rx"s) > 0) {
            for (auto& [s, mm] : modulemap) {
                if (count(mm->conn_str.begin(), mm->conn_str.end(), str) > 0) {
                    rx_grandfathers.push_back(s);
                }
            }
        }
    }

    vector<int64_t> gf_high(rx_grandfathers.size(), 0ll);

    queue<msg> msg_queue;
    for (size_t i = 1; i <= numeric_limits<size_t>::max(); ++i) {
        for (auto& s : modulemap["broadcaster"s]->conn_str) {
            msg_queue.push({"broadcaster"s, s, pulses::low});
        }

        int64_t rxlow  = 0;
        int64_t rxhigh = 0;

        while (!msg_queue.empty()) {
            auto& m = msg_queue.front();

            if (m.receiver == "rx"s) {
                if (m.pul == pulses::high) {
                    ++rxhigh;
                } else {
                    ++rxlow;
                }
            }

            auto it = rx_grandfathers.begin();
            if ((it = find(rx_grandfathers.begin(), rx_grandfathers.end(), m.sender)) != rx_grandfathers.end() && m.pul == pulses::high) {
                if (gf_high[it - rx_grandfathers.begin()] == 0) {
                    gf_high[it - rx_grandfathers.begin()] = i;
                }
            }

            if (!modulemap.contains(m.receiver)) {
                msg_queue.pop();
                continue;
            }

            if (modulemap[m.receiver]->mtype == MType::conjunct) {
                dynamic_cast<conjunct&>(*modulemap[m.receiver]).update_input(m.sender, m.pul);
            }

            modulemap[m.receiver]->handle_msg(m, msg_queue);

            msg_queue.pop();
        }

        if (count(gf_high.begin(), gf_high.end(), 0ll) == 0) {
            cout << accumulate(gf_high.begin(), gf_high.end(), 1LL, [](const int64_t a, const int64_t b) { return lcm(a, b); }) << endl;
            break;
        }

        if (rxlow == 1) {
            cout << i << endl;
            break;
        }
    }
}

int main() {
    part1();
    part2();
    return 0;
}