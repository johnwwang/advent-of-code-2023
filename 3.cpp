#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>

#define DIM 140

inline bool in_bounds(int i, int j) {
    return (i >= 0 && i < DIM && j >= 0 && j < DIM);
}
inline char get(const std::vector<char> &grid, int i, int j) {
    return in_bounds(i, j) ? grid[i * DIM + j] : '.';
}

inline void set(std::vector<char> &grid, int i, int j, char c) {
    if (in_bounds(i, j)) {
        grid[i * DIM + j] = c;  
    }
}

void print_grid(const std::vector<char> &grid) {
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            printf("%c", grid[i * DIM + j]);
        }
        printf("\n");
    }
}

void parse(std::vector<char> &v) {
    std::ifstream data("./data/3.txt");
    if (data.is_open()) {
        char c;
        for (int i = 0; i < DIM * DIM; ++i) {
            data >> c;
            v.push_back(c);
        }
    }
}

int extract_number(std::vector<char> &grid, int i, int j) {
    // go to end of the number
    // print_grid(grid);
    while (isdigit(get(grid, i, j + 1))) {
        ++j;
    }
    int num = 0;
    int place = 1;
    while (isdigit(get(grid, i, j))) {
        num += place * (get(grid, i, j) - '0');
        place *= 10;
        set(grid, i, j, ' '); // erase the number
        --j;
    }
    // printf("%d\n", num);
    return num;
}
// sum of engine numbers
// assume grid[i][j] is a symbol
// removes the numbers from the grid
// to prevent double counting
int get_engine_number_sum(std::vector<char> &grid, int i, int j) {
    int dir[3] = {-1, 0, 1};
    char c;

    int sum = 0;
    for (int di : dir) {
        for (int dj : dir) {
            c = get(grid, i + di, j + dj);
            if (isdigit(c)) {
                sum += extract_number(grid, i + di, j + dj);
            }
        }
    }
    return sum;
}

int solve_1() {
    int res = 0;

    std::vector<char> grid;
    parse(grid);
    // print_grid(grid);
    // printf("\n");
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            char c = get(grid, i, j);
            if (c != '.' && !isdigit(c)) {
                res += get_engine_number_sum(grid, i, j);
            }      
        }
    } 
    return res;
}

int extract_number_2(const std::vector<char> &grid, int i, int j) {
    // go to end of the number
    print_grid(grid);
    while (isdigit(get(grid, i, j + 1))) {
        ++j;
    }
    int num = 0;
    int place = 1;
    while (isdigit(get(grid, i, j))) {
        num += place * (get(grid, i, j) - '0');
        place *= 10;
        --j;
    }
    printf("%d\n", num);
    return num;
}



int get_gear_number(const std::vector<char> &grid, int i, int j) {
    char c;
    int num_gears = 0;
    int gear_number = 1;
    // top
    if (isdigit(get(grid, i - 1, j))) {
        num_gears++;
        gear_number *= extract_number_2(grid, i - 1, j);
    } else {
        if (isdigit(get(grid, i - 1, j - 1))) {
            num_gears++;
            gear_number *= extract_number_2(grid, i - 1, j - 1);
        }
        if (isdigit(get(grid, i - 1, j + 1))) {
            num_gears++;
            gear_number *= extract_number_2(grid, i - 1, j + 1);
        }
    }
    // bottom
    if (isdigit(get(grid, i + 1, j))) {
        num_gears++;
        gear_number *= extract_number_2(grid, i + 1, j);
    } else {
        if (isdigit(get(grid, i + 1, j - 1))) {
            num_gears++;
            gear_number *= extract_number_2(grid, i + 1, j - 1);
        }
        if (isdigit(get(grid, i + 1, j + 1))) {
            num_gears++;
            gear_number *= extract_number_2(grid, i + 1, j + 1);
        }
    }
    // left
    if (isdigit(get(grid, i, j - 1))) {
        num_gears++;
        gear_number *= extract_number_2(grid, i, j - 1);
    }
    //right
    if (isdigit(get(grid, i, j + 1))) {
        num_gears++;
        gear_number *= extract_number_2(grid, i, j + 1);
    }
    printf("num gears: %d, gear num %d\n", num_gears, gear_number);
    // std::cin.get();
    if (num_gears == 2){
        return gear_number;
    }
    return 0;
}

int solve_2() {
    int res = 0;
    std::vector<char> grid;
    parse(grid);
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            char c = get(grid, i, j);
            if (c == '*') {
                res += get_gear_number(grid, i, j);
            }      
        }
    } 
    return res;
    
}
int main() {
    printf("answer: %d\n", solve_2());
    return 0;
}