#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>

#define RED_COUNT 12
#define GREEN_COUNT 13
#define BLUE_COUNT 14

// for part 1
int process_1(FILE* fp, int id) {
    int cnt;
    char color[6];
    bool is_possible = true;
    while (fscanf(fp, "%d %s ", &cnt, color) == 2) {
        // printf("cnt: %d, color: %s\n", cnt, color);
        int max_cnt;

        switch (color[0]) {
            case 'r': {
                max_cnt = RED_COUNT;
                break;
            }
            case 'b': {
                max_cnt = BLUE_COUNT;
                break;
            }
            case 'g': {
                max_cnt = GREEN_COUNT; 
                break;
            } 
            default: {
                printf("hit default");
                return -1;
            }
        }

        if (cnt > max_cnt) {
            is_possible = false;
        }
    }
    return is_possible * id;
}

int process_2(FILE* fp, int id) {
    int cnt;
    char color[6];
    int min_r = 0, min_b = 0, min_g = 0;
    while (fscanf(fp, "%d %s ", &cnt, color) == 2) {
        switch(color[0]) {
            case 'r': {
                min_r = std::max(min_r, cnt);
                break;
            }
            case 'b': {
                min_b = std::max(min_b, cnt);
                break;
            }
            case 'g': {
                min_g = std::max(min_g, cnt);
                break;
            }
            default : {
                printf("default");
                return 0;
            }
        }
    }
    printf("id: %d, red: %d, blue: %d, green: %d\n",id , min_r, min_b, min_g);
    return min_r * min_b * min_g;
}
int main() {
    int res = 0;
    
    FILE* fp = fopen("./data/2.txt", "r");
    int id;
    
    while(fscanf(fp, "Game %d: ", &id) == 1) {
        res += process_2(fp, id);
    }
    printf("sum of possible ids: %d\n", res);
}