#include <cstdio>
#include <climits>
#include <array>
#include <vector>
#include <queue>
#include <string>
#include <utility>
#include <algorithm>
#include <unordered_map>

#define ull unsigned long long
#define TEST false

std::unordered_map<char, uint8_t> card_to_num = {
    {'J', 1},
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'T', 10},
    // {'J', 11},
    {'Q', 12},
    {'K', 13},
    {'A', 14}
};

bool cmp_hands(const std::pair<std::string, unsigned int> &h1, const std::pair<std::string, unsigned int> &h2) {
    const std::string &hand1 = h1.first, &hand2 = h2.first;

    for (int i = 0; i < 5; ++i) {
        if (card_to_num[hand1[i]] < card_to_num[hand2[i]]) return true;
        else if (card_to_num[hand1[i]] > card_to_num[hand2[i]]) return false;
    }
    return false;
}

void print_hand(uint8_t n) {
    printf("\t0b%c%c%c%c%c%c\n", 
        (n >> 5 & 1) + '0',
        (n >> 4 & 1) + '0',
        (n >> 3 & 1) + '0',
        (n >> 2 & 1) + '0',
        (n >> 1 & 1) + '0',
        (n & 1) + '0');
}

size_t get_bucket_idx(const std::string &s) {
    unsigned int count[14];
    std::fill(count, count + 14, 0);

    uint8_t max_cnt;
    for (const auto &c : s) {
        ++count[card_to_num[c] - 1];
    }
    uint8_t mask = 0;
    uint8_t temp = 0;
    uint8_t shift_target;
    
    // 0bqwerty -> y encodes the duplicate 2
    for (uint8_t i = 1; i < 14; ++i) {
        printf("count: %u ", count[i]);
        // if count is 0 we want to shift 0 (to do nothing) else shift 1
        shift_target = (count[i] | count[i] >> 1 | count[i] >> 2) & 1;

        temp = shift_target << count[i];
        mask |= (temp & mask) >> 2; // for duplicate two
        mask |= temp;
    }
    printf("mask before jokers: %u", mask);
    print_hand(mask);

    // get rightmost set bit
    temp = mask;
    temp = temp | (temp >> 1);
    temp = temp | (temp >> 2);
    temp = temp | (temp >> 4);
    temp |= 1; // for 0
    temp -= (temp >> 1);

    mask &= ~temp; // unset bit
    mask |= temp << count[0]; // add number of jokers

    // handle duplicate two
    mask |= (mask & 1) << (((count[0] | count[0] >> 1 | count[0] >> 2) & 1) << 1); // move the duplicate two (if it exists) to regular two
    mask &= ~((count[0] | count[0] >> 1 | count[0] >> 2) & 1); // unset the duplicate two if joker count > 0

    printf("mask after jokers: %u", mask);
    print_hand(mask);

    switch(mask & ~0b10) { // ignore 1s
        case 1U << 5: { // 5 hand
            return 0;
        }
        case 1U << 4: { // 4 1 hand
            return 1;
        }
        case 1U << 3 | 1U << 2: { // full house (3 2 hand)
            return 2;
        }
        case 1U << 3: { // triple (3 1 1 hand)
            return 3;
        }
        case 1U << 2 | 1U: { // 2 pair (2 1 2 hand)
            return 4;
        }
        case 1U << 2: { // pair (2 1 1 1 hand)
            return 5;
        }
        case 0: { // all different (1 1 1 1 1 hand)
            return 6;
        }
        default: {
            printf("error! %u\n", mask);
            return mask;
        }
    }
}

void a() {
    FILE* fp;
    if (TEST) {
        fp = fopen("./test/7.txt", "r");
    } else {
        fp = fopen("./data/7.txt", "r");
    }

    printf("string size: %ld, unsigned int size: %ld, pair size: %ld\n", 
        sizeof(std::string), 
        sizeof(unsigned int),
        sizeof(std::pair<std::string, char[12]>));

    uint32_t num_hands = 0;
    std::array<std::vector<std::pair<std::string, uint32_t>>, 7> hand_buckets;
    {
        char hand_buf[5];
        uint32_t bet;
        while (fscanf(fp, " %5c %u ", hand_buf, &bet) == 2) {
            printf("hand: %s, bet: %u\n", hand_buf, bet);
            hand_buckets[get_bucket_idx(hand_buf)].emplace_back(hand_buf, bet);
            ++num_hands;
        }
    }
    printf("\n");

    uint64_t res = 0;
    for (size_t i = 0; i < hand_buckets.size(); ++i) {
        printf("currently: %ld\n", i);
        std::sort(hand_buckets[i].begin(), hand_buckets[i].end(), cmp_hands);
        std::reverse(hand_buckets[i].begin(), hand_buckets[i].end());

        for (const auto &[hand, bet] : hand_buckets[i]) {
            res += bet * (num_hands--);
            printf("hand: %s, bet: %u ", hand.c_str(), bet);
        }
        printf("\n");
    }

    printf("res: %ld\n", res);
}

int main() {
    a();
    return 0;
};