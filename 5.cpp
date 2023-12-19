#include <stdio.h>
#include <fstream>
#include <cctype>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <iostream>
#include <algorithm> // std::min_element
#include <iterator>  // std::begin, std::end
#include <chrono>

#ifdef DEBUG
#define DEBUG_printf(x) printf("");
#else
#define DEBUG_printf(x) printf(x);
#endif

typedef unsigned long long ull;
typedef std::vector<std::vector<std::tuple<ull, ull, ull>>> vvtullullull;
 
// Interval(a, b) -> [a, b)
struct Interval {
    ull start;
    ull end;
    bool valid;
    Interval (ull start, ull end) : start(start), end(end), valid(true) {}
    Interval() : valid(false) {}
};

Interval intersection(Interval i1, Interval i2) {
    ull i_start = std::max(i1.start, i2.start);
    ull i_end = std::min(i1.end, i2.end);
    if (i_start > i_end) return Interval();
    return Interval(i_start, i_end);
}

std::pair<Interval, Interval> difference(Interval i1, Interval i2) {
    Interval isctn = intersection(i1, i2);
    if (!isctn.valid) return {i1, Interval()};

    Interval l = (isctn.start == i1.start) ? Interval() : Interval(i1.start, isctn.start); 
    Interval r = (isctn.end == i1.end) ? Interval() : Interval(isctn.end, i1.end);
    return {l, r};
}

void solve_naive() {
    FILE* fp = fopen("./test/5.txt", "r");
    std::vector<ull> seed_nums;
    std::vector<ull> seed_rngs;
    vvtullullull maps(7);
    
    fscanf(fp, "seeds: ");
    ull seed_num, seed_rng;
    while (fscanf(fp, " %lld %lld ", &seed_num, &seed_rng) == 2){
        // printf("%lld %lld", seed_num, seed_rng);
        seed_nums.push_back(seed_num);
        seed_rngs.push_back(seed_rng);
        printf("%lld+", seed_rng);
    }
    printf("\n");
    size_t idx = 0;
    char map_t[100];
    ull source_start, destination_start, range;
    while (fscanf(fp, " %s map: ", map_t) == 1) {
        printf("%s\n", map_t);
        printf("%lu\n", idx);
        while(fscanf(fp, " %lld %lld %lld ", &destination_start, &source_start, &range) == 3) {
            printf("%lld %lld %lld\n", destination_start, source_start, range);
            maps[idx].emplace_back(destination_start, source_start, range);
        }
        idx++;
    }
    printf("finished parsing\n");

    ull min_location = ULLONG_MAX;

    for (size_t i = 0; i < seed_nums.size(); ++i) {
        for (ull u = 0; u < seed_rngs[i]; ++u) {
            auto start = std::chrono::high_resolution_clock::now();
            ull curr = seed_nums[i] + u;
            printf(" seed_num: %lld ", curr);
            for (const auto &map : maps) {
                for (const auto &[d, s, r] : map) {
                    if (s <= curr && curr <= s + r) {
                        curr = d + curr - s;
                        break;
                    }
                }
                printf("%lld ", curr);
            }
            printf("\n");
            min_location = std::min(min_location, curr);
        }
    }
    printf("min location: %lld\n", min_location);
}

void solve_optimized() {
    FILE* fp = fopen("./data/5.txt", "r");
    std::vector<Interval> intervals;
    vvtullullull maps(7);
    
    fscanf(fp, "seeds: ");
    ull seed_num, seed_rng;
    while (fscanf(fp, " %lld %lld ", &seed_num, &seed_rng) == 2){
        intervals.emplace_back(seed_num, seed_num + seed_rng);
        printf("%lld %lld ", seed_num, seed_rng);
        printf("[%lld, %lld) ", seed_num, seed_num + seed_rng);
    }
    printf("\n");
    size_t idx = 0;
    char map_t[100];
    ull source_start, destination_start, range;
    while (fscanf(fp, " %s map: ", map_t) == 1) {
        printf("%s\n", map_t);
        printf("%lu\n", idx);
        while(fscanf(fp, " %lld %lld %lld ", &destination_start, &source_start, &range) == 3) {
            printf("%lld %lld %lld\n", destination_start, source_start, range);
            maps[idx].emplace_back(destination_start, source_start, range);
        }
        idx++;
    }
    printf("finished parsing\n");

    // push ranges through
    for (size_t i = 0; i < maps.size(); ++i) {
        printf("intervals:");
        for (const auto& interval : intervals) {
            printf(" [%lld, %lld)", interval.start, interval.end);
        }
        printf("\n");
        //get mapped ranges into new_intervals
        printf("starting map : %ld\n", i);
        std::vector<Interval> new_intervals;
        for (const auto &interval : intervals) {
            // initially only have starting range
            printf("\tstarting interval [%lld, %lld)\n", interval.start, interval.end);
            std::vector<Interval> interval_store = {interval};
            
            for (const auto &[d, s, r] : maps[i]) {
                printf("\t\tcurrent mapping: d: [%lld, %lld), s: [%lld, %lld)\n", d, d + r, s, s + r);
                std::vector<Interval> temp_interval_store;
                for (const auto &curr_interval : interval_store) {
                    printf("\t\t\tat interval: [%lld, %lld)\n", curr_interval.start, curr_interval.end);
                    Interval isctn = intersection(curr_interval, Interval(s, s + r));
                    auto [l_isctn, r_isctn] = difference(curr_interval, Interval(s, s + r));
                    if (isctn.valid) {
                        printf("\t\t\t\tintersection was: [%lld, %lld)\n", isctn.start, isctn.end);
                        if (d >= s) {
                            isctn.start += (d - s);
                            isctn.end += (d - s);
                        } else {
                            isctn.start -= (s - d);
                            isctn.end -= (s - d);
                        }
                        printf("\t\t\t\tintersection transformed: [%lld, %lld)\n", isctn.start, isctn.end);
                        new_intervals.push_back(isctn);
                    }

                    // printf("\t\t\tdifference was:");
                    if (l_isctn.valid) {
                        printf("\t\t\t\tleft [%lld, %lld)\n", l_isctn.start, l_isctn.end);
                        temp_interval_store.push_back(l_isctn);
                    }
                    if (r_isctn.valid) {
                        printf("\t\t\t\tright [%lld, %lld)\n", r_isctn.start, r_isctn.end);
                        temp_interval_store.push_back(r_isctn);
                    }
                }
                interval_store = std::move(temp_interval_store);
            }
            new_intervals.insert(new_intervals.begin(), interval_store.begin(), interval_store.end());
        }        

        //move new_intervals into rngs
        intervals = std::move(new_intervals);
    }
    ull res = ULONG_LONG_MAX;
    for (const auto &interval : intervals) {
        res = std::min(res, interval.start);
        printf("%lld(%lld) ", interval.start, interval.end);
    }
    printf("\n");
    printf("res: %lld\n", res);
}

int main() {
    // solve_naive();
    solve_optimized();
    return 0;
}