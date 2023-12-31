#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <vector>

#include "../../Utils/utils.cpp"

std::map<char, int> card_to_value;

void init_card_to_value(std::map<char, int> &map, std::string part) {
    for (int i = '2'; i <= '9'; i++) {
        map[i] = (i - '0');
    }
    map['T'] = 10;
    if (part == "1") {
        map['J'] = 11;
    } else {
        map['J'] = -1;
    }
    map['Q'] = 12;
    map['K'] = 13;
    map['A'] = 14;
}

enum Hand_Type {
    High_Card,
    One_Pair,
    Two_Pair,
    Three_Of_Kind,
    Full_House,
    Four_Of_Kind,
    Five_Of_Kind,
};

struct Hand {
    int bet;
    std::string cards;
    std::set<char> letters;
    Hand_Type type;
};

struct type_comp {
    // Operator() overloading
    bool operator()(const Hand &h1, const Hand &h2) {
        // new definition
        if (h1.type != h2.type) {
            return h1.type < h2.type;
        } else {
            for (int i = 0; i < h1.cards.length(); i++) {
                if (h1.cards[i] == h2.cards[i]) {
                    continue;
                } else {
                    return card_to_value[h1.cards[i]] < card_to_value[h2.cards[i]];
                }
            }
            return false;
        }
    }
};

void initialize_set_of_letters(std::string input, std::set<char> &charset) {
    for (auto a : input) {
        charset.insert(a);
    }
}

void analyze_hand_type(Hand &act_hand) {
    if (act_hand.letters.size() == 1) {
        act_hand.type = Five_Of_Kind;
    } else if (act_hand.letters.size() == 5) {
        act_hand.type = High_Card;
    } else if (act_hand.letters.size() == 4) {
        act_hand.type = One_Pair;
    } else if (act_hand.letters.size() == 3) {
        for (auto c : act_hand.letters) {
            auto char_count =
                std::count(act_hand.cards.begin(), act_hand.cards.end(), c);
            if (char_count == 3) {
                act_hand.type = Three_Of_Kind;
                break;
            } else {
                act_hand.type = Two_Pair;
            }
        }
    } else if (act_hand.letters.size() == 2) {
        for (auto c : act_hand.letters) {
            if (std::count(act_hand.cards.begin(), act_hand.cards.end(), c) == 1 ||
                std::count(act_hand.cards.begin(), act_hand.cards.end(), c) == 4) {
                act_hand.type = Four_Of_Kind;
            } else {
                act_hand.type = Full_House;
            }
        }
    }
}

void analyze_hand_type_part_2(Hand &act_hand) {
    int joker_count =
        std::count(act_hand.cards.begin(), act_hand.cards.end(), 'J');
    if (act_hand.letters.size() == 1 || joker_count == 4) {
        act_hand.type = Five_Of_Kind;
    } else if (act_hand.letters.size() == 5) {
        if (joker_count == 0) {
            act_hand.type = High_Card;
        } else {
            act_hand.type = One_Pair;
        }
    } else if (act_hand.letters.size() == 4) {
        if (joker_count == 0) {
            act_hand.type = One_Pair;
        } else {
            act_hand.type = Three_Of_Kind;
        }
    } else if (act_hand.letters.size() == 3) {
        act_hand.type = Two_Pair;
        for (auto c : act_hand.letters) {
            if (c == 'J') {
                continue;
            }
            auto char_count =
                std::count(act_hand.cards.begin(), act_hand.cards.end(), c) +
                joker_count;
            if (char_count == 3 && joker_count != 0) {
                act_hand.type = Full_House;
            } else if (char_count == 3 && joker_count == 0) {
                act_hand.type = Three_Of_Kind;
                break;
            } else if (char_count == 4) {
                act_hand.type = Four_Of_Kind;
                break;
            }
        }
    } else if (act_hand.letters.size() == 2) {
        for (auto c : act_hand.letters) {
            if (std::count(act_hand.cards.begin(), act_hand.cards.end(), c) == 1 ||
                std::count(act_hand.cards.begin(), act_hand.cards.end(), c) == 4) {
                if (joker_count != 0) {
                    act_hand.type = Five_Of_Kind;
                    break;
                } else {
                    act_hand.type = Four_Of_Kind;
                }
            } else {
                if (joker_count == 3 || joker_count == 2) {
                    act_hand.type = Five_Of_Kind;
                    break;
                } else {
                    act_hand.type = Full_House;
                }
            }
        }
    }
}

int main() {
    std::string input;
    std::vector<std::string> tokens;
    std::vector<Hand> hands;
    init_card_to_value(card_to_value, "2");
    while (getline(std::cin, input)) {
        tokens = split(' ', input);
        Hand act_hand = {std::stoi(tokens[1]), tokens[0]};
        initialize_set_of_letters(tokens[0], act_hand.letters);
        analyze_hand_type_part_2(act_hand);
        hands.emplace_back(act_hand);
    }
    type_comp tc;
    std::sort(hands.begin(), hands.end(), tc);
    long sum = 0;
    for (int i = 0; i < hands.size(); i++) {
        std::cout << hands[i].cards << " " << hands[i].bet << " " << hands[i].type << "\n";
        sum += hands[i].bet * (i + 1);
    }
    std::cout << sum;
}
