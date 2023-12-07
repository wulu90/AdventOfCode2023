#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

enum class type { highcard, onepair, twopairs, threekind, fullhouse, fourkind, fivekind };

map<char, int> card_conv{{'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}};

map<char, int> card_J_conv{{'T', 10}, {'J', 1}, {'Q', 12}, {'K', 13}, {'A', 14}};

struct hand {
    array<int, 5> cards;
    int bid;
    type t;
    int joker_num;

    hand(const string& str, bool joker = false) : joker_num(0), bid(0) {
        map<int, int> card_num;

        for (int i = 0; i < 5; ++i) {
            if (isdigit(str[i])) {
                cards[i] = str[i] - '0';
            } else {
                if (joker) {
                    cards[i] = card_J_conv[str[i]];
                } else {
                    cards[i] = card_conv[str[i]];
                }
                if (str[i] == 'J') {
                    ++joker_num;
                }
            }
            ++card_num[cards[i]];
        }

        bid = stoi(str.substr(6, str.size() - 6));

        auto n = card_num.size();

        if (n == 1) {
            t = type::fivekind;
        } else if (n == 2) {
            if (card_num.begin()->second == 4 || card_num.rbegin()->second == 4) {
                t = type::fourkind;
            } else {
                t = type::fullhouse;
            }

        } else if (n == 3) {
            bool isthree = false;
            for (auto [_, i] : card_num) {
                if (i == 3) {
                    isthree = true;
                    break;
                }
            }

            if (isthree) {
                t = type::threekind;
            } else {
                t = type::twopairs;
            }
        } else if (n == 4) {
            t = type::onepair;
        } else {
            t = type::highcard;
        }

        if (joker) {
            joker_change(card_num);
        }
    }

    void joker_change(const map<int, int>& card_num) {
        if (joker_num == 0 || t == type::fivekind) {
            return;
        }

        if (t == type::fourkind || t == type::fullhouse) {
            t = type::fivekind;
        } else if (t == type::threekind) {
            t = type::fourkind;
        } else if (t == type::twopairs) {
            if (joker_num == 2) {
                t = type::fourkind;
            } else {
                t = type::fullhouse;
            }
        } else if (t == type::onepair) {
            t = type::threekind;
        } else if (t == type::highcard) {
            t = type::onepair;
        }
    }
};

bool operator<(const hand& lhs, const hand& rhs) {
    if (lhs.t != rhs.t) {
        return lhs.t < rhs.t;
    } else {
        return lhs.cards < rhs.cards;
    }
}

void part1() {
    ifstream input("input");
    string str;
    vector<hand> hands;
    while (getline(input, str)) {
        hands.emplace_back(str);
    }

    sort(hands.begin(), hands.end());

    int total_winnings = 0;
    for (int i = 0; i < hands.size(); ++i) {
        total_winnings += hands[i].bid * (i + 1);
    }

    cout << total_winnings << endl;
}

void part2() {
    ifstream input("input");
    string str;
    vector<hand> hands;
    while (getline(input, str)) {
        hands.emplace_back(str, true);
    }

    sort(hands.begin(), hands.end());

    int total_winnings = 0;
    for (int i = 0; i < hands.size(); ++i) {
        total_winnings += hands[i].bid * (i + 1);
    }

    cout << total_winnings << endl;
}

int main() {
    part1();
    part2();
    return 0;
}