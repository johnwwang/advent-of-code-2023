#include <fstream>
#include <stdio.h>
#include <cctype>
#include <unordered_set>
#include <vector>

void solve_1() {
    FILE* fp = fopen("./data/4.txt", "r");
    int card_number;

    std::unordered_set<int> winning_numbers;

    int wn;
    char sep;
    int res = 0;
    while (!feof(fp)) {
        winning_numbers.clear();
        fscanf(fp, "Card %d: ", &card_number);
        while (fscanf(fp, " %d ", &wn) == 1) {
            winning_numbers.insert(wn);
        }
        if (fscanf(fp, " %c ", &sep) != 1) {
            return; 
        }
        if (sep != '|') {
            return;
        }
        int cnt = 0;
        while (fscanf(fp, " %d ", &wn) == 1) {
            if (winning_numbers.count(wn) == 1) {
                ++cnt;
            }
        }
        if (cnt) {
            res += 1 << (cnt - 1);
        }
        printf("card: %d, cnt: %d\n", card_number, cnt);
    }
    printf("result: %d\n", res);
    return;
}

void solve_2() {
    printf("solve2\n");
    FILE* fp = fopen("./data/4.txt", "r");
    
    long long count[218];
    for (int i = 0; i < 218; ++i) {
        count[i] = 1;
    }

    std::unordered_set<int> winning_numbers;
    
    while(!feof(fp)) {
        winning_numbers.clear();

        int card_number;
        fscanf(fp, "Card %d: ", &card_number);
        card_number;

        int num;
        while(fscanf(fp, " %d ", &num) == 1) {
            winning_numbers.insert(num);
        }

        char c;
        fscanf(fp, " %c ", &c);
        if (c != '|') return;
        
        int cnt = 0;
        while(fscanf(fp, " %d ", &num) == 1) {
            if (winning_numbers.count(num) == 1) {
                ++cnt;
            }
        }
        printf("card: %d, cnt: %d\n", card_number, cnt);
        while (cnt > 0) {
            count[card_number + cnt - 1] += count[card_number - 1];
            printf("card %d has number %lld\n", card_number + cnt, count[card_number+cnt-1]);
            cnt--;
        }
    }
    int res = 0;
    for (int i = 0; i < 218; ++i) {
        res += count[i];
    }
    printf("res: %d\n", res);
}

int main() {
    solve_2();
}