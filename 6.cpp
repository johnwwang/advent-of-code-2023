#include <cstdio>
#include <vector>
#include <climits>

#define ull unsigned long long
#define TEST false

int main() {
    FILE* fp;
    if (TEST) {
        fp = fopen("./test/6b.txt", "r");
    } else {
        fp = fopen("./data/6b.txt", "r");
    }
    
    fscanf(fp, "Time: ");

    ull a;

    std::vector<ull> times;
    while (fscanf(fp, " %lld ", &a) == 1) {
        times.push_back(a);
        printf("%lld ", a);
    }

    std::vector<ull> distances;

    fscanf(fp, "Distance: ");
    while (fscanf(fp, " %lld ", &a) == 1) {
        distances.push_back(a);
        printf("%lld ", a);
    }
    printf("parsed\n");

    if (distances.size() != times.size()) {
        printf("not equal\n");
        return 1;
    }

    for (size_t i = 0; i < times.size(); ++i) {
        ull distance = distances[i], time = times[i];
        ull charge_time = 0;
        for (; charge_time < time; ++charge_time) { // ignore divide by zero without using time
            if (charge_time >= ULLONG_MAX / (time - charge_time) || charge_time * (time - charge_time) > distance) break;
        }

        printf("charge_time: %lld, res: %lld\n", charge_time - 1, time - 2 * (charge_time) + 1);
    }
}