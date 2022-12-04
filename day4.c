#include <stdio.h>

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day4.txt", "r");

    int sa, ea, sb, eb;

    int full_count = 0;
    int partial_count = 0;

    while(fscanf(file, "%d-%d,%d-%d", &sa, &ea, &sb, &eb)==4) {
        if ((sa >= sb && ea <= eb) || (sb >= sa && eb <= ea)) {
            full_count += 1;
        }
        if ((sa <= sb && ea >= sb) || (sa <= eb && ea >= eb) || (sb <= sa && eb >= sa) || (sb <= ea && eb >= ea)) {
            partial_count += 1;
        }
    }
    printf("Count: %d\n", full_count);
    printf("Partial count: %d\n", partial_count);

    return 0;
}