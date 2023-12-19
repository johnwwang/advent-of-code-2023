#include <stdio.h>
#include <string>
#include <iostream>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> _numbers = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

unsigned int get_calibration_value(const std::string &line) {
    // printf("line: %s\n", line.c_str());
    size_t first_found_numeric_str_loc = std::string::npos;
    size_t last_found_numeric_str_loc = 0; // will always find one digit or numeric str

    uint8_t first_found_number, last_found_number;

    for (size_t i = 0; i < _numbers.size(); ++i) {
        std::string &s = _numbers[i];
        size_t first_str_loc = line.find(s);
        size_t last_str_loc = line.rfind(s);

        if (first_str_loc != std::string::npos && first_str_loc < first_found_numeric_str_loc) {
            first_found_numeric_str_loc = first_str_loc;
            first_found_number = i + 1;
        }
        if (last_str_loc != std::string::npos && last_str_loc >= last_found_numeric_str_loc) {
            last_found_numeric_str_loc = last_str_loc;
            last_found_number = i + 1;
        }
    }

    size_t first_found_digit_loc = line.find_first_of("123456789");
    size_t last_found_digit_loc = line.find_last_of("123456789");

    if (first_found_digit_loc != std::string::npos && first_found_digit_loc < first_found_numeric_str_loc) {
        first_found_number = line[first_found_digit_loc] - '0';
        // printf("found digit: %c", line[first_found_digit_loc]);
    }
    if (last_found_digit_loc != std::string::npos && last_found_digit_loc >= last_found_numeric_str_loc) {
        last_found_number = line[last_found_digit_loc] - '0';
        // printf("found digit: %c", line[last_found_digit_loc]);
    }
    return 10 * first_found_number + last_found_number;
}

int main() {
    int sum = 0;
    FILE* fp = fopen("./data/1.txt", "r");

    unsigned int ans;
    char s[100];
    std::fill(s, s + 100, '\0');
    size_t sz;
    
    while (fscanf(fp, "%s%ln ", s, &sz) == 1) {
        if (sz > 100) {
            printf("sz greater than 100: %ld\n",sz);
            return 1;
        }
        printf("input: %s, sz: %lu\n", s, sz);
        ans += get_calibration_value(s);
        printf("calibration value: %u\n", get_calibration_value(s));
    }
    printf("ans: %u\n", ans);
    return 0;
} 